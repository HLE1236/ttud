import os
import re
import sys
import uuid
import time
import queue
import shutil
import tempfile
import threading
import subprocess


def find_gdb():
    env_gdb = os.environ.get("GDB_PATH")
    if env_gdb:
        return shutil.which(env_gdb) or (env_gdb if os.path.exists(env_gdb) else None)
    if os.name == "nt":
        win_gdb = r"C:\msys64\ucrt64\bin\gdb.exe"
        if os.path.exists(win_gdb):
            return win_gdb
        return shutil.which("gdb")
    for cand in ("/opt/homebrew/bin/gdb", "/usr/local/bin/gdb"):
        if os.path.isfile(cand) and os.access(cand, os.X_OK):
            return cand
    return shutil.which("gdb")


def find_lldb():
    env_lldb = os.environ.get("LLDB_PATH")
    if env_lldb:
        return shutil.which(env_lldb) or (env_lldb if os.path.exists(env_lldb) else None)
    if sys.platform == "darwin":
        for cand in ("/usr/bin/lldb", "/opt/homebrew/opt/llvm/bin/lldb", "/usr/local/opt/llvm/bin/lldb"):
            if os.path.isfile(cand) and os.access(cand, os.X_OK):
                return cand
    return shutil.which("lldb")


GDB_PATH = find_gdb()
LLDB_PATH = find_lldb()


def resolve_debugger_backend():
    forced = (os.environ.get("DEBUGGER_BACKEND") or "").strip().lower()
    if forced == "lldb" and LLDB_PATH:
        return "lldb", LLDB_PATH
    if forced == "gdb" and GDB_PATH:
        return "gdb", GDB_PATH
    if sys.platform == "darwin":
        if LLDB_PATH:
            return "lldb", LLDB_PATH
        if GDB_PATH:
            return "gdb", GDB_PATH
        return None, None
    if GDB_PATH:
        return "gdb", GDB_PATH
    if LLDB_PATH:
        return "lldb", LLDB_PATH
    return None, None


ACTIVE_BACKEND, ACTIVE_DEBUGGER_PATH = resolve_debugger_backend()


# ---------------------------------------------------------------------------
# Minimal GDB/MI2 output parser
# ---------------------------------------------------------------------------
def _parse_cstring(s, i):
    assert s[i] == '"'
    i += 1
    out = []
    n = len(s)
    while i < n:
        ch = s[i]
        if ch == '"':
            return "".join(out), i + 1
        if ch == "\\" and i + 1 < n:
            nxt = s[i + 1]
            if nxt == "n":
                out.append("\n")
                i += 2
            elif nxt == "t":
                out.append("\t")
                i += 2
            elif nxt == "r":
                out.append("\r")
                i += 2
            elif nxt in ('"', "\\"):
                out.append(nxt)
                i += 2
            elif nxt in "01234567":
                # Octal escape \xxx
                j = i + 1
                while j < n and j < i + 4 and s[j] in "01234567":
                    j += 1
                try:
                    out.append(chr(int(s[i + 1 : j], 8)))
                except ValueError:
                    out.append(s[i:j])
                i = j
            else:
                out.append(nxt)
                i += 2
        else:
            out.append(ch)
            i += 1
    return "".join(out), i


def _parse_value(s, i):
    n = len(s)
    if i >= n:
        return "", i
    ch = s[i]
    if ch == '"':
        return _parse_cstring(s, i)
    if ch == "{":
        return _parse_tuple(s, i)
    if ch == "[":
        return _parse_list(s, i)
    # Fallback: read until delimiter
    j = i
    while j < n and s[j] not in ",}]":
        j += 1
    return s[i:j], j


def _parse_result(s, i):
    n = len(s)
    eq = s.find("=", i)
    if eq == -1:
        return None, None, n
    key = s[i:eq].strip()
    val, nxt = _parse_value(s, eq + 1)
    return key, val, nxt


def _parse_tuple(s, i):
    assert s[i] == "{"
    i += 1
    n = len(s)
    res = {}
    while i < n and s[i] != "}":
        if s[i] == ",":
            i += 1
            continue
        k, v, i = _parse_result(s, i)
        if k is not None:
            res[k] = v
    return res, (i + 1 if i < n and s[i] == "}" else i)


def _parse_list(s, i):
    assert s[i] == "["
    i += 1
    n = len(s)
    res = []
    while i < n and s[i] != "]":
        if s[i] == ",":
            i += 1
            continue
        if s[i] in ('"', "{", "["):
            v, i = _parse_value(s, i)
            res.append(v)
        else:
            # Named item in list, e.g. frame={...}
            k, v, i = _parse_result(s, i)
            if k is not None:
                res.append({k: v})
    return res, (i + 1 if i < n and s[i] == "]" else i)


def parse_mi_body(body):
    res = {}
    i = 0
    n = len(body)
    while i < n:
        if body[i] == ",":
            i += 1
            continue
        k, v, i = _parse_result(body, i)
        if k is not None:
            res[k] = v
        else:
            break
    return res


def mi_escape(text):
    return (
        str(text)
        .replace("\\", "\\\\")
        .replace('"', '\\"')
        .replace("\n", " ")
        .replace("\r", " ")
    )


# ---------------------------------------------------------------------------
# GDB Debug Session
# ---------------------------------------------------------------------------
class DebugSession:
    def __init__(self, gxx_path, compat_inc_dir, judge_env):
        self.id = uuid.uuid4().hex
        self.gxx_path = gxx_path
        self.compat_inc_dir = compat_inc_dir
        self.judge_env = judge_env
        self.tmpdir = tempfile.mkdtemp(prefix="ttud_dbg_")
        self.src_path = os.path.join(self.tmpdir, "main.cpp")
        self.exe_path = os.path.join(self.tmpdir, "main.exe" if os.name == "nt" else "main")
        self.in_path = os.path.join(self.tmpdir, "in.txt")
        self.out_path = os.path.join(self.tmpdir, "out.txt")

        self.proc = None
        self.lock = threading.Lock()
        self.line_queue = queue.Queue()
        self.reader_thread = None

        self.breakpoints = {}  # line (int) -> bkpt number (str)
        self.watches = []
        self.logs = []
        self.status = "idle"  # paused | exited | error
        self.reason = ""
        self.signal_name = ""
        self.current_line = None
        self.current_func = ""
        self.in_user_file = False
        self.last_active = time.time()

    def _log(self, msg):
        msg = msg.rstrip("\r\n")
        if not msg:
            return
        self.logs.append(msg)
        if len(self.logs) > 200:
            self.logs = self.logs[-200:]

    def _reader_loop(self):
        try:
            for raw in self.proc.stdout:
                line = raw.rstrip("\r\n")
                self.line_queue.put(line)
        except Exception:
            pass
        finally:
            self.line_queue.put(None)

    def _send(self, cmd):
        if not self.proc or self.proc.poll() is not None:
            raise RuntimeError("Phiên GDB đã kết thúc.")
        self.proc.stdin.write(cmd + "\n")
        self.proc.stdin.flush()

    def _read_until_prompt(self, timeout=5.0):
        """Read MI lines until (gdb) prompt; return (result_record, stopped_record)."""
        deadline = time.time() + timeout
        result = None
        stopped = None
        while True:
            rem = deadline - time.time()
            if rem <= 0:
                raise TimeoutError("GDB không phản hồi kịp thời.")
            try:
                line = self.line_queue.get(timeout=min(rem, 0.25))
            except queue.Empty:
                continue
            if line is None:
                break
            if line == "(gdb)" or line == "(gdb) ":
                if result is not None or stopped is not None:
                    break
                continue
            if line.startswith("~"):
                try:
                    txt, _ = _parse_cstring(line, 1)
                    if not txt.startswith("[New Thread") and not txt.startswith("[Thread "):
                        self._log(txt)
                except Exception:
                    pass
            elif line.startswith("&"):
                pass
            elif line.startswith("^"):
                comma = line.find(",")
                cls = line[1:] if comma == -1 else line[1:comma]
                body = parse_mi_body(line[comma + 1 :]) if comma != -1 else {}
                result = {"class": cls, "body": body}
            elif line.startswith("*stopped"):
                comma = line.find(",")
                body = parse_mi_body(line[comma + 1 :]) if comma != -1 else {}
                stopped = body
        return result, stopped

    def _exec_and_wait_stop(self, cmd, timeout=10.0):
        """Send an execution command (run/next/step/finish/continue) and wait for *stopped."""
        self._send(cmd)
        deadline = time.time() + timeout
        stopped = None
        err_msg = None
        while True:
            rem = deadline - time.time()
            if rem <= 0:
                self.close()
                self.status = "exited"
                self.reason = "timeout"
                self._log("Quá thời gian 10s (có thể vòng lặp vô hạn) — đã dừng phiên debug.")
                return None, "Chương trình chạy quá 10 giây (có thể bị vòng lặp vô hạn) — đã dừng phiên debug."
            try:
                line = self.line_queue.get(timeout=min(rem, 0.2))
            except queue.Empty:
                continue
            if line is None:
                self.status = "exited"
                self.reason = "exited"
                return None, None
            if line.startswith("~"):
                try:
                    txt, _ = _parse_cstring(line, 1)
                    if not txt.startswith("[New Thread") and not txt.startswith("[Thread "):
                        self._log(txt)
                except Exception:
                    pass
            elif line.startswith("^error"):
                comma = line.find(",")
                body = parse_mi_body(line[comma + 1 :]) if comma != -1 else {}
                err_msg = body.get("msg", "Lỗi GDB")
                self._log(f"GDB Error: {err_msg}")
                return None, err_msg
            elif line.startswith("*stopped"):
                comma = line.find(",")
                stopped = parse_mi_body(line[comma + 1 :]) if comma != -1 else {}
                # Drain trailing (gdb) prompt briefly
                try:
                    while True:
                        nxt = self.line_queue.get(timeout=0.05)
                        if nxt is None or nxt.strip() == "(gdb)":
                            break
                except queue.Empty:
                    pass
                return stopped, None

    def _mi_cmd(self, cmd, timeout=4.0):
        self._send(cmd)
        res, _ = self._read_until_prompt(timeout=timeout)
        return res or {"class": "error", "body": {"msg": "No response"}}

    def _is_user_file(self, fname):
        if not fname:
            return False
        base = os.path.basename(fname.replace("\\", "/"))
        return base.lower() == "main.cpp"

    def _update_from_stopped(self, stopped):
        if not stopped:
            return
        reason = stopped.get("reason", "")
        self.reason = reason
        self.signal_name = stopped.get("signal-name", "")
        if reason in ("exited-normally", "exited", "exited-signalled"):
            self.status = "exited"
            self.current_line = None
            self.current_func = ""
            self.in_user_file = False
            exit_code = stopped.get("exit-code", "0")
            self._log(f"Chương trình kết thúc ({reason}, exit code {exit_code}).")
            return

        self.status = "paused"
        frame = stopped.get("frame", {})
        if isinstance(frame, dict):
            self.current_func = frame.get("func", "")
            fname = frame.get("fullname") or frame.get("file") or ""
            self.in_user_file = self._is_user_file(fname)
            try:
                self.current_line = int(frame.get("line")) if frame.get("line") else None
            except ValueError:
                self.current_line = None
        if self.signal_name:
            meaning = stopped.get("signal-meaning", "")
            self._log(f"Nhận tín hiệu {self.signal_name} ({meaning}) tại dòng {self.current_line or '?'}")

    def _read_stdout(self):
        if not os.path.exists(self.out_path):
            return ""
        try:
            with open(self.out_path, "r", encoding="utf-8", errors="replace") as f:
                data = f.read(32768)
            return data
        except OSError:
            return ""

    def _collect_state(self, error_msg=None):
        self.last_active = time.time()
        locals_list = []
        frames_list = []
        watches_list = []

        if self.status == "paused" and self.proc and self.proc.poll() is None:
            # 1. Locals: get types with --simple-values and pretty values with --all-values
            types_map = {}
            r_types = self._mi_cmd("-stack-list-variables --simple-values")
            if r_types.get("class") == "done":
                for v in r_types["body"].get("variables", []):
                    if isinstance(v, dict) and v.get("name"):
                        types_map[v["name"]] = v.get("type", "")

            r_vals = self._mi_cmd("-stack-list-variables --all-values")
            if r_vals.get("class") == "done":
                for v in r_vals["body"].get("variables", []):
                    if isinstance(v, dict) and v.get("name"):
                        name = v["name"]
                        val = v.get("value", "")
                        if len(val) > 300:
                            val = val[:300] + " ..."
                        locals_list.append({
                            "name": name,
                            "type": types_map.get(name, ""),
                            "value": val,
                        })

            # 2. Call stack frames
            r_frames = self._mi_cmd("-stack-list-frames 0 25")
            if r_frames.get("class") == "done":
                for item in r_frames["body"].get("stack", []):
                    fr = item.get("frame", item) if isinstance(item, dict) else {}
                    fname = fr.get("fullname") or fr.get("file") or ""
                    try:
                        ln = int(fr.get("line")) if fr.get("line") else None
                    except ValueError:
                        ln = None
                    frames_list.append({
                        "level": int(fr.get("level", 0)),
                        "func": fr.get("func", "??"),
                        "file": os.path.basename(fname.replace("\\", "/")) if fname else "",
                        "line": ln,
                        "in_user_file": self._is_user_file(fname),
                    })

            # 3. User watches
            for expr in self.watches:
                e = expr.strip()
                if not e:
                    continue
                r_w = self._mi_cmd(f'-data-evaluate-expression "{mi_escape(e)}"')
                if r_w.get("class") == "done":
                    val = r_w["body"].get("value", "")
                    if len(val) > 300:
                        val = val[:300] + " ..."
                    watches_list.append({"expr": e, "value": val, "error": ""})
                else:
                    watches_list.append({
                        "expr": e,
                        "value": "",
                        "error": r_w["body"].get("msg", "Không thể tính biểu thức"),
                    })

        return {
            "ok": error_msg is None,
            "error": error_msg or "",
            "session_id": self.id,
            "status": self.status,
            "reason": self.reason,
            "signal": self.signal_name,
            "line": self.current_line if self.in_user_file else None,
            "raw_line": self.current_line,
            "func": self.current_func,
            "in_user_file": self.in_user_file,
            "locals": locals_list,
            "watches": watches_list,
            "frames": frames_list,
            "breakpoints": sorted(self.breakpoints.keys()),
            "stdout": self._read_stdout(),
            "logs": self.logs[-60:],
        }

    def start(self, code, stdin_text, breakpoints, watches):
        if not GDB_PATH:
            return {
                "ok": False,
                "error": "Không tìm thấy GDB trên máy. Hãy cài gdb (Windows MSYS2: pacman -S mingw-w64-ucrt-x86_64-gdb; macOS: brew install gdb; Linux: sudo apt install gdb).",
            }

        with open(self.src_path, "w", encoding="utf-8") as f:
            f.write(code)
        with open(self.in_path, "w", encoding="utf-8") as f:
            f.write(stdin_text or "")
        with open(self.out_path, "w", encoding="utf-8") as f:
            f.write("")

        cmd = [self.gxx_path, "-g", "-O0", "-std=c++17", "-Wall"]
        if self.compat_inc_dir and os.path.isdir(self.compat_inc_dir):
            cmd.extend(["-idirafter", self.compat_inc_dir])
        cmd.extend([self.src_path, "-o", self.exe_path])

        try:
            comp = subprocess.run(
                cmd,
                env=self.judge_env,
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
                timeout=25,
            )
        except FileNotFoundError:
            self.close()
            return {"ok": False, "error": f"Không tìm thấy trình biên dịch C++ ({self.gxx_path})."}
        except subprocess.TimeoutExpired:
            self.close()
            return {"ok": False, "error": "Biên dịch quá thời gian (25s)."}

        if comp.returncode != 0:
            self.close()
            return {
                "ok": False,
                "compile_error": comp.stderr or comp.stdout or "Lỗi biên dịch",
                "error": "Lỗi biên dịch (-g -O0)",
            }

        self.watches = [w for w in (watches or []) if isinstance(w, str) and w.strip()]

        try:
            self.proc = subprocess.Popen(
                [GDB_PATH, "--quiet", "--interpreter=mi2", self.exe_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                encoding="utf-8",
                errors="replace",
                env=self.judge_env,
                cwd=self.tmpdir,
                bufsize=1,
            )
        except Exception as e:
            self.close()
            return {"ok": False, "error": f"Không khởi chạy được GDB: {e}"}

        self.reader_thread = threading.Thread(target=self._reader_loop, daemon=True)
        self.reader_thread.start()

        # Wait for initial (gdb) prompt
        deadline = time.time() + 5.0
        while time.time() < deadline:
            try:
                line = self.line_queue.get(timeout=0.2)
                if line is None or line.strip() == "(gdb)":
                    break
            except queue.Empty:
                break

        self._mi_cmd("-enable-pretty-printing")
        self._mi_cmd("-gdb-set print elements 40")

        # Insert breakpoints
        valid_bps = sorted({int(ln) for ln in (breakpoints or []) if isinstance(ln, int) and ln > 0})
        for ln in valid_bps:
            res = self._mi_cmd(f"-break-insert main.cpp:{ln}")
            if res.get("class") == "done":
                bkpt = res["body"].get("bkpt", {})
                if isinstance(bkpt, dict) and bkpt.get("number"):
                    actual_ln = int(bkpt.get("line", ln))
                    self.breakpoints[actual_ln] = bkpt["number"]

        # If no breakpoint was set, pause at entry of main()
        if not self.breakpoints:
            self._mi_cmd("-break-insert -t main")

        stopped, err = self._exec_and_wait_stop('-interpreter-exec console "run < in.txt > out.txt"')
        if err:
            return self._collect_state(error_msg=err)
        self._update_from_stopped(stopped)
        return self._collect_state()

    def action(self, act, line=None, level=None):
        with self.lock:
            if self.status != "paused":
                return self._collect_state(error_msg="Phiên debug không ở trạng thái tạm dừng.")
            if act == "frame" and level is not None:
                res = self._mi_cmd(f"-stack-select-frame {int(level)}")
                if res.get("class") == "done":
                    r_info = self._mi_cmd("-stack-info-frame")
                    if r_info.get("class") == "done":
                        fr = r_info["body"].get("frame", {})
                        fname = fr.get("fullname") or fr.get("file") or ""
                        self.current_func = fr.get("func", "")
                        self.in_user_file = self._is_user_file(fname)
                        try:
                            self.current_line = int(fr.get("line")) if fr.get("line") else None
                        except ValueError:
                            self.current_line = None
                return self._collect_state()

            if act == "next":
                stopped, err = self._exec_and_wait_stop("-exec-next")
            elif act == "step":
                stopped, err = self._exec_and_wait_stop("-exec-step")
            elif act == "finish":
                stopped, err = self._exec_and_wait_stop("-exec-finish")
            elif act == "continue":
                stopped, err = self._exec_and_wait_stop("-exec-continue")
            elif act == "until" and line:
                self._mi_cmd(f"-break-insert -t main.cpp:{int(line)}")
                stopped, err = self._exec_and_wait_stop("-exec-continue")
            else:
                return self._collect_state(error_msg=f"Hành động debug không hợp lệ: {act}")

            if err:
                return self._collect_state(error_msg=err)
            self._update_from_stopped(stopped)
            return self._collect_state()

    def sync_breakpoints(self, lines):
        with self.lock:
            if self.status != "paused":
                return self._collect_state()
            desired = {int(ln) for ln in (lines or []) if isinstance(ln, int) and ln > 0}
            # Remove deleted breakpoints
            for ln in list(self.breakpoints.keys()):
                if ln not in desired:
                    self._mi_cmd(f"-break-delete {self.breakpoints[ln]}")
                    del self.breakpoints[ln]
            # Add new breakpoints
            for ln in sorted(desired):
                if ln not in self.breakpoints:
                    res = self._mi_cmd(f"-break-insert main.cpp:{ln}")
                    if res.get("class") == "done":
                        bkpt = res["body"].get("bkpt", {})
                        if isinstance(bkpt, dict) and bkpt.get("number"):
                            actual_ln = int(bkpt.get("line", ln))
                            self.breakpoints[actual_ln] = bkpt["number"]
            return self._collect_state()

    def set_watches(self, watches):
        with self.lock:
            self.watches = [w for w in (watches or []) if isinstance(w, str) and w.strip()]
            return self._collect_state()

    def close(self):
        proc = self.proc
        self.proc = None
        if proc is not None:
            try:
                if proc.poll() is None:
                    try:
                        proc.stdin.write("-gdb-exit\n")
                        proc.stdin.flush()
                    except Exception:
                        pass
                    try:
                        proc.wait(timeout=1.0)
                    except subprocess.TimeoutExpired:
                        proc.kill()
            except Exception:
                try:
                    proc.kill()
                except Exception:
                    pass
        if self.tmpdir and os.path.isdir(self.tmpdir):
            shutil.rmtree(self.tmpdir, ignore_errors=True)


# ---------------------------------------------------------------------------
# LLDB Debug Session (macOS native / LLVM LLDB)
# ---------------------------------------------------------------------------
def _parse_lldb_frame_line(line):
    """Parse a single LLDB frame line like:
    * frame #0: 0x0000000100003e40 main`add(x=10, y=25) at main.cpp:4:9
    """
    m = re.search(r"frame\s+#(\d+):\s+0x[0-9a-fA-F]+\s+(.*)", line)
    if not m:
        return None
    level = int(m.group(1))
    rest = m.group(2).strip()

    file_name = ""
    line_num = None
    m_at = re.search(r"\s+at\s+([^:\n]+):(\d+)", rest)
    if m_at:
        file_name = os.path.basename(m_at.group(1).strip().replace("\\", "/"))
        try:
            line_num = int(m_at.group(2))
        except ValueError:
            line_num = None
        rest = rest[: m_at.start()].strip()

    # Strip module prefix like main`
    if "`" in rest:
        rest = rest.split("`", 1)[1]
    # Strip + offset and argument list
    func = re.sub(r"\s*\+\s*\d+.*$", "", rest).strip()
    paren = func.find("(")
    if paren != -1:
        func = func[:paren].strip()
    func = func or "??"
    return {
        "level": level,
        "func": func,
        "file": file_name,
        "line": line_num,
        "in_user_file": file_name.lower() == "main.cpp",
    }


def _parse_lldb_variables(output_text):
    """Parse LLDB `frame variable` output into [{name, type, value}]."""
    result = []
    cur = None
    top_re = re.compile(r"^\((.+?)\)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(.*)$")

    def flush():
        nonlocal cur
        if not cur:
            return
        val = cur["value"].strip()
        if len(val) > 300:
            val = val[:300] + " ..."
        result.append({"name": cur["name"], "type": cur["type"], "value": val})
        cur = None

    for raw in output_text.splitlines():
        if raw.startswith("(lldb)") or not raw.strip():
            continue
        if not raw[0].isspace():
            m = top_re.match(raw)
            if m:
                flush()
                cur = {"type": m.group(1).strip(), "name": m.group(2).strip(), "value": m.group(3).strip()}
                continue
        if cur is not None:
            piece = raw.strip()
            if piece in ("{", "}"):
                cur["value"] = (cur["value"] + " " + piece).strip()
            else:
                sep = ", " if (cur["value"] and not cur["value"].endswith("{")) else " "
                cur["value"] += sep + piece
    flush()
    return result


def _parse_lldb_expression(output_text):
    """Parse LLDB `expression -- <expr>` output -> (value, error)."""
    lines = [ln for ln in output_text.splitlines() if ln.strip() and not ln.startswith("(lldb)")]
    errs = [ln for ln in lines if ln.startswith("error:") or "error:" in ln[:20]]
    if errs:
        return "", errs[0].replace("error:", "").strip()
    if not lines:
        return "", "Không có giá trị trả về"
    first = lines[0]
    m = re.match(r"^\(.+?\)\s+\$\d+\s*=\s*(.*)$", first)
    val = m.group(1).strip() if m else first.strip()
    if len(lines) > 1:
        extra = " ".join(ln.strip() for ln in lines[1:])
        val = (val + " " + extra).strip()
    if len(val) > 300:
        val = val[:300] + " ..."
    return val, ""


class LldbDebugSession:
    def __init__(self, gxx_path, compat_inc_dir, judge_env):
        self.id = uuid.uuid4().hex
        self.gxx_path = gxx_path
        self.compat_inc_dir = compat_inc_dir
        self.judge_env = judge_env
        self.tmpdir = tempfile.mkdtemp(prefix="ttud_dbg_")
        self.src_path = os.path.join(self.tmpdir, "main.cpp")
        self.exe_path = os.path.join(self.tmpdir, "main.exe" if os.name == "nt" else "main")
        self.in_path = os.path.join(self.tmpdir, "in.txt")
        self.out_path = os.path.join(self.tmpdir, "out.txt")

        self.proc = None
        self.lock = threading.Lock()
        self.line_queue = queue.Queue()
        self.reader_thread = None

        self.breakpoints = {}  # line (int) -> bkpt id (str)
        self.watches = []
        self.logs = []
        self.status = "idle"
        self.reason = ""
        self.signal_name = ""
        self.current_line = None
        self.current_func = ""
        self.in_user_file = False
        self.last_active = time.time()

    def _log(self, msg):
        for ln in str(msg).splitlines():
            s = ln.rstrip("\r\n")
            if s and not s.startswith("(lldb)"):
                self.logs.append(s)
        if len(self.logs) > 200:
            self.logs = self.logs[-200:]

    def _reader_loop(self):
        try:
            for raw in self.proc.stdout:
                self.line_queue.put(raw.rstrip("\r\n"))
        except Exception:
            pass
        finally:
            self.line_queue.put(None)

    def _run_cmd(self, cmd, timeout=10.0):
        """Send an LLDB command followed by a unique sentinel and collect output."""
        if not self.proc or self.proc.poll() is not None:
            raise RuntimeError("Phiên LLDB đã kết thúc.")
        tok = f"__TTUD_MARK_{uuid.uuid4().hex[:10]}__"
        self.proc.stdin.write(f"{cmd}\nhelp {tok}\n")
        self.proc.stdin.flush()

        deadline = time.time() + timeout
        out_lines = []
        while True:
            rem = deadline - time.time()
            if rem <= 0:
                self.close()
                self.status = "exited"
                self.reason = "timeout"
                raise TimeoutError("Chương trình chạy quá 10 giây (có thể bị vòng lặp vô hạn) — đã dừng phiên debug.")
            try:
                line = self.line_queue.get(timeout=min(rem, 0.2))
            except queue.Empty:
                continue
            if line is None:
                break
            if tok in line:
                break
            if line.startswith("(lldb) "):
                continue
            out_lines.append(line)
        return "\n".join(out_lines)

    def _read_stdout(self):
        if not os.path.exists(self.out_path):
            return ""
        try:
            with open(self.out_path, "r", encoding="utf-8", errors="replace") as f:
                return f.read(32768)
        except OSError:
            return ""

    def _update_status_from_output(self, out_text):
        self._log(out_text)
        self.signal_name = ""
        m_exit = re.search(r"Process\s+\d+\s+exited\s+with\s+status\s*=\s*(-?\d+)", out_text)
        if m_exit:
            self.status = "exited"
            self.reason = "exited-normally"
            self.current_line = None
            self.current_func = ""
            self.in_user_file = False
            return

        if "stop reason =" in out_text or "Process " in out_text and "stopped" in out_text:
            self.status = "paused"
            m_reason = re.search(r"stop reason\s*=\s*(.+)", out_text)
            if m_reason:
                r = m_reason.group(1).strip()
                self.reason = r
                if any(sig in r for sig in ("EXC_", "SIG", "signal")):
                    self.signal_name = r.split()[0]
        else:
            # Query process status directly if output was terse
            st = self._run_cmd("process status", timeout=4.0)
            if "exited with status" in st or "No process" in st:
                self.status = "exited"
                self.reason = "exited-normally"
                self.current_line = None
                self.current_func = ""
                self.in_user_file = False
                return
            self.status = "paused"

        # Determine active frame
        fr_out = self._run_cmd("frame info", timeout=4.0)
        parsed = None
        for ln in fr_out.splitlines():
            parsed = _parse_lldb_frame_line(ln)
            if parsed:
                break
        if parsed:
            self.current_func = parsed["func"]
            self.current_line = parsed["line"]
            self.in_user_file = parsed["in_user_file"]

    def _collect_state(self, error_msg=None):
        self.last_active = time.time()
        locals_list = []
        frames_list = []
        watches_list = []

        if self.status == "paused" and self.proc and self.proc.poll() is None:
            try:
                var_out = self._run_cmd("frame variable", timeout=4.0)
                locals_list = _parse_lldb_variables(var_out)

                bt_out = self._run_cmd("thread backtrace -c 25", timeout=4.0)
                for ln in bt_out.splitlines():
                    fr = _parse_lldb_frame_line(ln)
                    if fr:
                        frames_list.append(fr)

                for expr in self.watches:
                    e = expr.strip()
                    if not e:
                        continue
                    w_out = self._run_cmd(f"expression -- {e}", timeout=4.0)
                    val, err = _parse_lldb_expression(w_out)
                    watches_list.append({"expr": e, "value": val, "error": err})
            except Exception as e:
                error_msg = error_msg or str(e)

        return {
            "ok": error_msg is None,
            "error": error_msg or "",
            "session_id": self.id,
            "status": self.status,
            "reason": self.reason,
            "signal": self.signal_name,
            "line": self.current_line if self.in_user_file else None,
            "raw_line": self.current_line,
            "func": self.current_func,
            "in_user_file": self.in_user_file,
            "locals": locals_list,
            "watches": watches_list,
            "frames": frames_list,
            "breakpoints": sorted(self.breakpoints.keys()),
            "stdout": self._read_stdout(),
            "logs": self.logs[-60:],
        }

    def start(self, code, stdin_text, breakpoints, watches):
        if not LLDB_PATH:
            return {
                "ok": False,
                "error": "Không tìm thấy LLDB trên máy. Trên macOS, hãy chạy: xcode-select --install",
            }

        with open(self.src_path, "w", encoding="utf-8") as f:
            f.write(code)
        with open(self.in_path, "w", encoding="utf-8") as f:
            f.write(stdin_text or "")
        with open(self.out_path, "w", encoding="utf-8") as f:
            f.write("")

        cmd = [self.gxx_path, "-g", "-O0", "-std=c++17", "-Wall"]
        if self.compat_inc_dir and os.path.isdir(self.compat_inc_dir):
            cmd.extend(["-idirafter", self.compat_inc_dir])
        cmd.extend([self.src_path, "-o", self.exe_path])

        try:
            comp = subprocess.run(
                cmd,
                env=self.judge_env,
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
                timeout=25,
            )
        except FileNotFoundError:
            self.close()
            return {"ok": False, "error": f"Không tìm thấy trình biên dịch C++ ({self.gxx_path})."}
        except subprocess.TimeoutExpired:
            self.close()
            return {"ok": False, "error": "Biên dịch quá thời gian (25s)."}

        if comp.returncode != 0:
            self.close()
            return {
                "ok": False,
                "compile_error": comp.stderr or comp.stdout or "Lỗi biên dịch",
                "error": "Lỗi biên dịch (-g -O0)",
            }

        self.watches = [w for w in (watches or []) if isinstance(w, str) and w.strip()]

        try:
            self.proc = subprocess.Popen(
                [LLDB_PATH, "--no-lldbinit", self.exe_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                encoding="utf-8",
                errors="replace",
                env=self.judge_env,
                cwd=self.tmpdir,
                bufsize=1,
            )
        except Exception as e:
            self.close()
            return {"ok": False, "error": f"Không khởi chạy được LLDB: {e}"}

        self.reader_thread = threading.Thread(target=self._reader_loop, daemon=True)
        self.reader_thread.start()

        try:
            self._run_cmd("settings set use-color false", timeout=5.0)
            self._run_cmd("settings set auto-confirm true", timeout=4.0)
            self._run_cmd("settings set stop-line-count-before 0", timeout=4.0)
            self._run_cmd("settings set stop-line-count-after 0", timeout=4.0)
            self._run_cmd("settings set target.max-children-count 40", timeout=4.0)

            valid_bps = sorted({int(ln) for ln in (breakpoints or []) if isinstance(ln, int) and ln > 0})
            for ln in valid_bps:
                bp_out = self._run_cmd(f"breakpoint set --file main.cpp --line {ln}", timeout=4.0)
                m = re.search(r"Breakpoint\s+(\d+):", bp_out)
                if m:
                    m_ln = re.search(r"main\.cpp:(\d+)", bp_out)
                    actual_ln = int(m_ln.group(1)) if m_ln else ln
                    self.breakpoints[actual_ln] = m.group(1)

            if not self.breakpoints:
                self._run_cmd("breakpoint set --one-shot true --name main", timeout=4.0)

            run_out = self._run_cmd("process launch -i in.txt -o out.txt", timeout=10.0)
            self._update_status_from_output(run_out)
            return self._collect_state()
        except TimeoutError as e:
            return self._collect_state(error_msg=str(e))
        except Exception as e:
            self.close()
            return {"ok": False, "error": f"Lỗi LLDB: {e}"}

    def action(self, act, line=None, level=None):
        with self.lock:
            if self.status != "paused":
                return self._collect_state(error_msg="Phiên debug không ở trạng thái tạm dừng.")
            try:
                if act == "frame" and level is not None:
                    self._run_cmd(f"frame select {int(level)}", timeout=4.0)
                    fr_out = self._run_cmd("frame info", timeout=4.0)
                    for ln in fr_out.splitlines():
                        parsed = _parse_lldb_frame_line(ln)
                        if parsed:
                            self.current_func = parsed["func"]
                            self.current_line = parsed["line"]
                            self.in_user_file = parsed["in_user_file"]
                            break
                    return self._collect_state()

                if act == "next":
                    out = self._run_cmd("thread step-over", timeout=10.0)
                elif act == "step":
                    out = self._run_cmd("thread step-in", timeout=10.0)
                elif act == "finish":
                    out = self._run_cmd("thread step-out", timeout=10.0)
                elif act == "continue":
                    out = self._run_cmd("process continue", timeout=10.0)
                elif act == "until" and line:
                    self._run_cmd(f"breakpoint set --one-shot true --file main.cpp --line {int(line)}", timeout=4.0)
                    out = self._run_cmd("process continue", timeout=10.0)
                else:
                    return self._collect_state(error_msg=f"Hành động debug không hợp lệ: {act}")

                self._update_status_from_output(out)
                return self._collect_state()
            except TimeoutError as e:
                return self._collect_state(error_msg=str(e))
            except Exception as e:
                return self._collect_state(error_msg=f"Lỗi LLDB: {e}")

    def sync_breakpoints(self, lines):
        with self.lock:
            if self.status != "paused":
                return self._collect_state()
            desired = {int(ln) for ln in (lines or []) if isinstance(ln, int) and ln > 0}
            for ln in list(self.breakpoints.keys()):
                if ln not in desired:
                    self._run_cmd(f"breakpoint delete {self.breakpoints[ln]}", timeout=4.0)
                    del self.breakpoints[ln]
            for ln in sorted(desired):
                if ln not in self.breakpoints:
                    bp_out = self._run_cmd(f"breakpoint set --file main.cpp --line {ln}", timeout=4.0)
                    m = re.search(r"Breakpoint\s+(\d+):", bp_out)
                    if m:
                        m_ln = re.search(r"main\.cpp:(\d+)", bp_out)
                        actual_ln = int(m_ln.group(1)) if m_ln else ln
                        self.breakpoints[actual_ln] = m.group(1)
            return self._collect_state()

    def set_watches(self, watches):
        with self.lock:
            self.watches = [w for w in (watches or []) if isinstance(w, str) and w.strip()]
            return self._collect_state()

    def close(self):
        proc = self.proc
        self.proc = None
        if proc is not None:
            try:
                if proc.poll() is None:
                    try:
                        proc.stdin.write("quit\n")
                        proc.stdin.flush()
                    except Exception:
                        pass
                    try:
                        proc.wait(timeout=1.0)
                    except subprocess.TimeoutExpired:
                        proc.kill()
            except Exception:
                try:
                    proc.kill()
                except Exception:
                    pass
        if self.tmpdir and os.path.isdir(self.tmpdir):
            shutil.rmtree(self.tmpdir, ignore_errors=True)


class DebugManager:
    def __init__(self, gxx_path, compat_inc_dir, judge_env_fn):
        self.gxx_path = gxx_path
        self.compat_inc_dir = compat_inc_dir
        self.judge_env_fn = judge_env_fn
        self.sessions = {}
        self.lock = threading.Lock()

    def _cleanup_stale(self):
        now = time.time()
        stale = [sid for sid, s in self.sessions.items() if now - s.last_active > 900]
        for sid in stale:
            s = self.sessions.pop(sid, None)
            if s:
                s.close()

    def start(self, code, stdin_text, breakpoints, watches, prev_session_id=None):
        with self.lock:
            self._cleanup_stale()
            if prev_session_id and prev_session_id in self.sessions:
                old = self.sessions.pop(prev_session_id)
                old.close()
            # Keep at most 4 concurrent sessions
            while len(self.sessions) >= 4:
                oldest_id = min(self.sessions, key=lambda k: self.sessions[k].last_active)
                self.sessions.pop(oldest_id).close()

        backend, _ = resolve_debugger_backend()
        if backend == "lldb":
            sess = LldbDebugSession(self.gxx_path, self.compat_inc_dir, self.judge_env_fn())
        else:
            sess = DebugSession(self.gxx_path, self.compat_inc_dir, self.judge_env_fn())
        res = sess.start(code, stdin_text, breakpoints, watches)
        if res.get("ok") and sess.status == "paused":
            with self.lock:
                self.sessions[sess.id] = sess
        else:
            sess.close()
        return res

    def get(self, session_id):
        with self.lock:
            return self.sessions.get(session_id)

    def stop(self, session_id):
        with self.lock:
            sess = self.sessions.pop(session_id, None)
        if sess:
            sess.close()
        return {"ok": True, "status": "stopped"}

    def action(self, session_id, act, line=None, level=None):
        sess = self.get(session_id)
        if not sess:
            return {"ok": False, "error": "Phiên debug không tồn tại hoặc đã kết thúc."}
        res = sess.action(act, line=line, level=level)
        if sess.status != "paused":
            self.stop(session_id)
        return res

    def sync_breakpoints(self, session_id, lines):
        sess = self.get(session_id)
        if not sess:
            return {"ok": False, "error": "Không có phiên debug đang chạy."}
        return sess.sync_breakpoints(lines)

    def set_watches(self, session_id, watches):
        sess = self.get(session_id)
        if not sess:
            return {"ok": False, "error": "Không có phiên debug đang chạy."}
        return sess.set_watches(watches)
