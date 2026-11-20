import os
import re
import sys
import json
import glob
import time
import shutil
import socket
import sqlite3
import tempfile
import mimetypes
import subprocess
from datetime import datetime
from urllib.parse import urlparse, parse_qs
from http.server import ThreadingHTTPServer, BaseHTTPRequestHandler
import debugger

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.join(BASE_DIR, "data")
PROBLEMS_DIR = os.path.join(DATA_DIR, "problems")
STATIC_DIR = os.path.join(BASE_DIR, "static")
DB_PATH = os.path.join(DATA_DIR, "ttud.db")
LECTURE_PDF_DIR = (
    os.environ.get("LECTURE_PDF_DIR")
    or (
        os.path.join(os.path.dirname(BASE_DIR), "pdf-shared")
        if os.path.isdir(os.path.join(os.path.dirname(BASE_DIR), "pdf-shared"))
        else r"D:\2026.1\TTUD\pdf-shared"
    )
)
COMPAT_INC_DIR = os.path.join(BASE_DIR, "compat")

if os.name == "nt":
    try:
        import ctypes
        # SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX: prevent Windows Error Reporting from hanging crashed C++ child processes
        ctypes.windll.kernel32.SetErrorMode(0x0001 | 0x0002)
    except Exception:
        pass

MINGW_BIN = r"C:\msys64\ucrt64\bin"


def find_gxx():
    env_cxx = os.environ.get("GXX_PATH") or os.environ.get("CXX")
    if env_cxx:
        return shutil.which(env_cxx) or env_cxx

    if os.name == "nt":
        win_gxx = r"C:\msys64\ucrt64\bin\g++.exe"
        if os.path.exists(win_gxx):
            return win_gxx
        return shutil.which("g++") or "g++"

    if sys.platform == "darwin":
        # Prefer GNU g++ from Homebrew (has native <bits/stdc++.h>) before Apple Clang
        search_dirs = ["/opt/homebrew/bin", "/usr/local/bin"]
        path_dirs = os.environ.get("PATH", "").split(os.pathsep)
        all_dirs = search_dirs + [d for d in path_dirs if d and d not in search_dirs]
        for ver in ("15", "14", "13", "12", "11"):
            for d in all_dirs:
                cand = os.path.join(d, f"g++-{ver}")
                if os.path.isfile(cand) and os.access(cand, os.X_OK):
                    return cand
        for d in all_dirs:
            cand = os.path.join(d, "g++")
            if os.path.isfile(cand) and os.access(cand, os.X_OK):
                return cand
        return shutil.which("g++") or shutil.which("clang++") or "g++"

    return shutil.which("g++") or shutil.which("clang++") or "g++"


GXX_PATH = find_gxx()


def now_iso():
    return datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def get_judge_env():
    env = os.environ.copy()
    if os.path.isdir(MINGW_BIN):
        env["PATH"] = MINGW_BIN + os.pathsep + env.get("PATH", "")
    return env


DEBUG_MANAGER = debugger.DebugManager(GXX_PATH, COMPAT_INC_DIR, get_judge_env)


def get_db():
    conn = sqlite3.connect(DB_PATH, timeout=15)
    conn.row_factory = sqlite3.Row
    return conn


def init_db():
    os.makedirs(DATA_DIR, exist_ok=True)
    conn = get_db()
    cur = conn.cursor()
    cur.execute("PRAGMA journal_mode=WAL;")
    cur.execute("""
        CREATE TABLE IF NOT EXISTS submissions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            problem_id TEXT NOT NULL,
            code TEXT NOT NULL,
            verdict TEXT NOT NULL,
            passed INTEGER NOT NULL,
            total INTEGER NOT NULL,
            max_time_ms INTEGER NOT NULL,
            compile_error TEXT DEFAULT '',
            details_json TEXT DEFAULT '[]',
            created_at TEXT NOT NULL
        )
    """)
    cur.execute("""
        CREATE TABLE IF NOT EXISTS problem_status (
            problem_id TEXT PRIMARY KEY,
            solved INTEGER DEFAULT 0,
            attempts INTEGER DEFAULT 0,
            best_time_ms INTEGER,
            last_verdict TEXT DEFAULT '',
            last_submitted_at TEXT
        )
    """)
    cur.execute("""
        CREATE TABLE IF NOT EXISTS notes (
            problem_id TEXT PRIMARY KEY,
            content TEXT NOT NULL DEFAULT '',
            updated_at TEXT
        )
    """)
    cur.execute("""
        CREATE TABLE IF NOT EXISTS drafts (
            problem_id TEXT PRIMARY KEY,
            code TEXT NOT NULL DEFAULT '',
            updated_at TEXT
        )
    """)
    cur.execute("""
        CREATE TABLE IF NOT EXISTS roadmap_checked (
            day INTEGER PRIMARY KEY,
            checked INTEGER DEFAULT 0,
            updated_at TEXT
        )
    """)
    cur.execute("""
        CREATE TABLE IF NOT EXISTS settings (
            key TEXT PRIMARY KEY,
            value TEXT NOT NULL
        )
    """)
    cur.execute("""
        CREATE TABLE IF NOT EXISTS exam_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            exam_id TEXT NOT NULL,
            title TEXT NOT NULL,
            track TEXT NOT NULL,
            total_score REAL NOT NULL,
            duration_used_sec INTEGER NOT NULL,
            problems_json TEXT NOT NULL,
            feedback TEXT DEFAULT '',
            created_at TEXT NOT NULL
        )
    """)
    cur.execute("""
        CREATE TABLE IF NOT EXISTS mistakes (
            problem_id TEXT PRIMARY KEY,
            last_verdict TEXT NOT NULL DEFAULT '',
            tag TEXT NOT NULL DEFAULT '',
            note TEXT NOT NULL DEFAULT '',
            fail_count INTEGER NOT NULL DEFAULT 1,
            resolved INTEGER NOT NULL DEFAULT 0,
            updated_at TEXT NOT NULL
        )
    """)
    conn.commit()
    conn.close()


def safe_pid(pid):
    if not pid or not re.match(r"^[a-zA-Z0-9_-]+$", pid):
        return None
    pdir = os.path.join(PROBLEMS_DIR, pid)
    if not os.path.isdir(pdir):
        return None
    return pdir


def load_all_problem_summaries():
    items = []
    if not os.path.isdir(PROBLEMS_DIR):
        return items
    for pid in sorted(os.listdir(PROBLEMS_DIR)):
        meta_path = os.path.join(PROBLEMS_DIR, pid, "meta.json")
        if os.path.isfile(meta_path):
            with open(meta_path, "r", encoding="utf-8") as f:
                m = json.load(f)
            pattern = ""
            guide_path = os.path.join(PROBLEMS_DIR, pid, "guide.json")
            if os.path.isfile(guide_path):
                try:
                    with open(guide_path, "r", encoding="utf-8") as gf:
                        pattern = json.load(gf).get("pattern", "")
                except Exception:
                    pass
            has_gen = os.path.isfile(os.path.join(PROBLEMS_DIR, pid, "gen.py"))
            items.append({
                "id": m["id"],
                "title": m["title"],
                "track": m["track"],
                "topic": m["topic"],
                "pattern": pattern,
                "has_gen": has_gen,
                "difficulty": m["difficulty"],
                "elo": m.get("elo", 1200),
                "exam_tags": m.get("exam_tags", []),
                "time_limit_ms": m.get("time_limit_ms", 1000),
                "has_official_tests": m.get("has_official_tests", False),
                "has_checker": m.get("has_checker", False),
                "test_count": m.get("test_count", 0),
                "theory_chapter": m.get("theory_chapter", "chap1"),
            })
    return items


def normalize_output(s):
    lines = [line.rstrip() for line in s.replace("\r\n", "\n").replace("\r", "\n").split("\n")]
    while lines and lines[-1] == "":
        lines.pop()
    return "\n".join(lines)


def preview_text(s, limit=600):
    s = s.replace("\r\n", "\n")
    if len(s) <= limit:
        return s
    return s[:limit] + f"\n... (cắt bớt, tổng {len(s)} ký tự)"


def compile_cpp(code, tmpdir, name="main"):
    src_path = os.path.join(tmpdir, f"{name}.cpp")
    exe_name = f"{name}.exe" if os.name == "nt" else name
    exe_path = os.path.join(tmpdir, exe_name)
    with open(src_path, "w", encoding="utf-8") as f:
        f.write(code)
    cmd = [GXX_PATH, "-O2", "-std=c++17", "-Wall"]
    if os.name == "nt":
        cmd.append("-Wl,--stack,268435456")
    if os.path.isdir(COMPAT_INC_DIR):
        cmd.extend(["-idirafter", COMPAT_INC_DIR])
    cmd.extend([src_path, "-o", exe_path])
    try:
        comp = subprocess.run(
            cmd,
            env=get_judge_env(),
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
            timeout=25,
        )
    except FileNotFoundError:
        return None, f"Không tìm thấy trình biên dịch C++ ({GXX_PATH}). Hãy cài g++ hoặc Xcode Command Line Tools."
    except subprocess.TimeoutExpired:
        return None, "Biên dịch quá thời gian (25s)."
    if comp.returncode != 0:
        return None, comp.stderr or comp.stdout or "Lỗi biên dịch không xác định"
    return exe_path, ""


def compute_stats(problems, status_map):
    total = len(problems)
    solved = 0
    midterm_total = midterm_solved = final_total = final_solved = 0
    by_topic = {}
    for p in problems:
        st = status_map.get(p["id"], {})
        is_ac = bool(st.get("solved", 0))
        if is_ac:
            solved += 1
        if p["track"] == "midterm":
            midterm_total += 1
            if is_ac:
                midterm_solved += 1
        else:
            final_total += 1
            if is_ac:
                final_solved += 1
        t = p["topic"]
        if t not in by_topic:
            by_topic[t] = {"total": 0, "solved": 0}
        by_topic[t]["total"] += 1
        if is_ac:
            by_topic[t]["solved"] += 1
    return {
        "total": total,
        "solved": solved,
        "midterm_total": midterm_total,
        "midterm_solved": midterm_solved,
        "final_total": final_total,
        "final_solved": final_solved,
        "by_topic": by_topic,
    }


def judge_run_sample(pid, code, custom_input):
    pdir = safe_pid(pid)
    if not pdir:
        return {"error": "Không tìm thấy bài tập"}, 404
    with open(os.path.join(pdir, "meta.json"), "r", encoding="utf-8") as f:
        meta = json.load(f)
    tl_sec = max(0.5, meta.get("time_limit_ms", 1000) / 1000.0)

    with tempfile.TemporaryDirectory(ignore_cleanup_errors=True) as tmpdir:
        exe_path, err = compile_cpp(code, tmpdir)
        if not exe_path:
            return {"verdict": "CE", "compile_error": err, "output": "", "time_ms": 0}, 200
        t0 = time.perf_counter()
        try:
            res = subprocess.run(
                [exe_path],
                input=custom_input,
                env=get_judge_env(),
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
                timeout=tl_sec,
            )
            dt_ms = int((time.perf_counter() - t0) * 1000)
            if res.returncode != 0:
                return {
                    "verdict": "RE",
                    "output": preview_text(res.stdout, 4000),
                    "stderr": preview_text(res.stderr or f"Exit code {res.returncode}", 1500),
                    "time_ms": dt_ms,
                }, 200
            return {
                "verdict": "OK",
                "output": preview_text(res.stdout, 8000),
                "stderr": preview_text(res.stderr, 1000),
                "time_ms": dt_ms,
            }, 200
        except subprocess.TimeoutExpired:
            dt_ms = int((time.perf_counter() - t0) * 1000)
            return {
                "verdict": "TLE",
                "output": "",
                "stderr": f"Vượt quá giới hạn thời gian ({meta.get('time_limit_ms', 1000)} ms)",
                "time_ms": dt_ms,
            }, 200


def judge_submit(pid, code):
    pdir = safe_pid(pid)
    if not pdir:
        return {"error": "Không tìm thấy bài tập"}, 404
    with open(os.path.join(pdir, "meta.json"), "r", encoding="utf-8") as f:
        meta = json.load(f)
    tl_ms = meta.get("time_limit_ms", 1000)
    tl_sec = max(0.5, tl_ms / 1000.0)
    tests_dir = os.path.join(pdir, "tests")
    in_files = sorted(glob.glob(os.path.join(tests_dir, "*.in")))
    checker_path = os.path.join(pdir, "checker.py")
    has_checker = os.path.isfile(checker_path)
    total = len(in_files)

    with tempfile.TemporaryDirectory(ignore_cleanup_errors=True) as tmpdir:
        exe_path, cerr = compile_cpp(code, tmpdir)
        if not exe_path:
            verdict = "CE"
            passed = 0
            max_time = 0
            details = []
        else:
            details = []
            passed = 0
            max_time = 0
            overall = "AC"
            tle_count = 0
            env = get_judge_env()

            for inf in in_files:
                tname = os.path.splitext(os.path.basename(inf))[0]
                outf = os.path.splitext(inf)[0] + ".out"
                if tle_count >= 2:
                    details.append({
                        "test": tname,
                        "verdict": "SKIP",
                        "time_ms": 0,
                        "note": "Bỏ qua vì đã TLE quá 2 test trước đó",
                    })
                    continue

                with open(inf, "r", encoding="utf-8", errors="replace") as fi:
                    inp_data = fi.read()
                exp_data = ""
                if os.path.isfile(outf):
                    with open(outf, "r", encoding="utf-8", errors="replace") as fo:
                        exp_data = fo.read()

                t0 = time.perf_counter()
                try:
                    res = subprocess.run(
                        [exe_path],
                        input=inp_data,
                        env=env,
                        capture_output=True,
                        text=True,
                        encoding="utf-8",
                        errors="replace",
                        timeout=tl_sec,
                    )
                    dt_ms = int((time.perf_counter() - t0) * 1000)
                    max_time = max(max_time, dt_ms)
                    if res.returncode != 0:
                        if overall == "AC":
                            overall = "RE"
                        details.append({
                            "test": tname,
                            "verdict": "RE",
                            "time_ms": dt_ms,
                            "input_preview": preview_text(inp_data, 500),
                            "expected_preview": preview_text(exp_data, 500),
                            "actual_preview": preview_text(res.stderr or f"Runtime Error (Exit code {res.returncode})", 500),
                        })
                        continue

                    if has_checker:
                        act_path = os.path.join(tmpdir, f"act_{tname}.out")
                        with open(act_path, "w", encoding="utf-8") as fa:
                            fa.write(res.stdout)
                        chk = subprocess.run(
                            [sys.executable, checker_path, inf, outf, act_path],
                            capture_output=True,
                            text=True,
                            encoding="utf-8",
                            errors="replace",
                            timeout=10,
                        )
                        if chk.returncode == 0:
                            passed += 1
                            details.append({"test": tname, "verdict": "AC", "time_ms": dt_ms})
                        else:
                            if overall == "AC":
                                overall = "WA"
                            details.append({
                                "test": tname,
                                "verdict": "WA",
                                "time_ms": dt_ms,
                                "input_preview": preview_text(inp_data, 500),
                                "expected_preview": preview_text(exp_data, 500),
                                "actual_preview": preview_text(res.stdout, 500),
                                "checker_msg": (chk.stdout or chk.stderr or "").strip(),
                            })
                    else:
                        if normalize_output(res.stdout) == normalize_output(exp_data):
                            passed += 1
                            details.append({"test": tname, "verdict": "AC", "time_ms": dt_ms})
                        else:
                            if overall == "AC":
                                overall = "WA"
                            details.append({
                                "test": tname,
                                "verdict": "WA",
                                "time_ms": dt_ms,
                                "input_preview": preview_text(inp_data, 500),
                                "expected_preview": preview_text(exp_data, 500),
                                "actual_preview": preview_text(res.stdout, 500),
                            })
                except subprocess.TimeoutExpired:
                    dt_ms = int((time.perf_counter() - t0) * 1000)
                    max_time = max(max_time, dt_ms)
                    tle_count += 1
                    if overall == "AC":
                        overall = "TLE"
                    details.append({
                        "test": tname,
                        "verdict": "TLE",
                        "time_ms": dt_ms,
                        "input_preview": preview_text(inp_data, 500),
                        "expected_preview": preview_text(exp_data, 500),
                        "actual_preview": f"Quá thời gian cho phép ({tl_ms} ms)",
                    })
            verdict = overall

    ts = now_iso()
    conn = get_db()
    cur = conn.cursor()
    cur.execute(
        """
        INSERT INTO submissions (problem_id, code, verdict, passed, total, max_time_ms, compile_error, details_json, created_at)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
        """,
        (pid, code, verdict, passed, total, max_time, cerr, json.dumps(details, ensure_ascii=False), ts),
    )
    sub_id = cur.lastrowid

    cur.execute("SELECT solved, attempts, best_time_ms FROM problem_status WHERE problem_id = ?", (pid,))
    row = cur.fetchone()
    prev_solved = row["solved"] if row else 0
    prev_attempts = row["attempts"] if row else 0
    prev_best = row["best_time_ms"] if row else None
    new_solved = 1 if (prev_solved or verdict == "AC") else 0
    new_best = prev_best
    if verdict == "AC":
        new_best = max_time if prev_best is None else min(prev_best, max_time)

    cur.execute(
        """
        INSERT INTO problem_status (problem_id, solved, attempts, best_time_ms, last_verdict, last_submitted_at)
        VALUES (?, ?, ?, ?, ?, ?)
        ON CONFLICT(problem_id) DO UPDATE SET
            solved = excluded.solved,
            attempts = excluded.attempts,
            best_time_ms = excluded.best_time_ms,
            last_verdict = excluded.last_verdict,
            last_submitted_at = excluded.last_submitted_at
        """,
        (pid, new_solved, prev_attempts + 1, new_best, verdict, ts),
    )
    if verdict != "AC":
        cur.execute(
            """
            INSERT INTO mistakes (problem_id, last_verdict, tag, note, fail_count, resolved, updated_at)
            VALUES (?, ?, '', '', 1, 0, ?)
            ON CONFLICT(problem_id) DO UPDATE SET
                last_verdict = excluded.last_verdict,
                fail_count = mistakes.fail_count + 1,
                resolved = 0,
                updated_at = excluded.updated_at
            """,
            (pid, verdict, ts),
        )
    else:
        cur.execute(
            "UPDATE mistakes SET resolved = 1, updated_at = ? WHERE problem_id = ?",
            (ts, pid),
        )
    conn.commit()

    # Recompute stats + recent history
    cur.execute("SELECT * FROM problem_status")
    status_map = {r["problem_id"]: dict(r) for r in cur.fetchall()}
    cur.execute("SELECT * FROM mistakes WHERE problem_id = ?", (pid,))
    mrow = cur.fetchone()
    mistake_rec = dict(mrow) if mrow else None
    problems = load_all_problem_summaries()
    stats = compute_stats(problems, status_map)
    conn.close()

    return {
        "submission_id": sub_id,
        "problem_id": pid,
        "verdict": verdict,
        "passed": passed,
        "total": total,
        "max_time_ms": max_time,
        "compile_error": cerr,
        "details": details,
        "created_at": ts,
        "problem_status": status_map.get(pid),
        "mistake": mistake_rec,
        "stats": stats,
    }, 200


def judge_stress_test(pid, code, max_iters=60):
    pdir = safe_pid(pid)
    if not pdir:
        return {"error": "Không tìm thấy bài tập"}, 404
    gen_path = os.path.join(pdir, "gen.py")
    ref_path = os.path.join(pdir, "ref.cpp")
    if not os.path.isfile(gen_path) or not os.path.isfile(ref_path):
        return {"error": "Bài này chưa có bộ sinh test ngẫu nhiên (gen.py) hoặc ref.cpp."}, 400
    checker_path = os.path.join(pdir, "checker.py")
    has_checker = os.path.isfile(checker_path)
    with open(os.path.join(pdir, "meta.json"), "r", encoding="utf-8") as f:
        meta = json.load(f)
    tl_sec = max(0.5, meta.get("time_limit_ms", 1000) / 1000.0)
    with open(ref_path, "r", encoding="utf-8", errors="replace") as rf:
        ref_code = rf.read()

    max_iters = max(1, min(int(max_iters or 60), 150))
    with tempfile.TemporaryDirectory(ignore_cleanup_errors=True) as tmpdir:
        user_exe, cerr = compile_cpp(code, tmpdir, "user")
        if not user_exe:
            return {"status": "CE", "compile_error": cerr}, 200
        ref_exe, rerr = compile_cpp(ref_code, tmpdir, "ref")
        if not ref_exe:
            return {"error": f"Lỗi biên dịch ref.cpp: {rerr}"}, 500

        env = get_judge_env()
        gen_dir = os.path.join(tmpdir, "gen_tests")
        os.makedirs(gen_dir, exist_ok=True)
        try:
            g_res = subprocess.run(
                [sys.executable, gen_path, gen_dir],
                capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=15,
            )
        except Exception as e:
            return {"error": f"Lỗi chạy gen.py: {e}"}, 500
        if g_res.returncode != 0:
            return {"error": f"gen.py lỗi: {g_res.stderr or g_res.stdout}"}, 500

        in_files = sorted(glob.glob(os.path.join(gen_dir, "*.in")))
        if not in_files:
            return {"error": "gen.py không sinh ra file .in nào."}, 500

        t_start = time.perf_counter()
        tested = 0
        for it, inf in enumerate(in_files[:max_iters], 1):
            if time.perf_counter() - t_start > 15.0:
                return {"status": "PASSED", "iters": tested, "note": "Dừng do đạt ngưỡng thời gian 15s."}, 200
            inp_data = open(inf, "r", encoding="utf-8", errors="replace").read()

            try:
                r_res = subprocess.run(
                    [ref_exe], input=inp_data, env=env,
                    capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=max(2.0, tl_sec),
                )
            except Exception as e:
                return {"error": f"ref.cpp lỗi ở test #{it}: {e}"}, 500
            exp_data = r_res.stdout

            try:
                u_res = subprocess.run(
                    [user_exe], input=inp_data, env=env,
                    capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=tl_sec,
                )
            except subprocess.TimeoutExpired:
                return {
                    "status": "FOUND", "iter": it, "seed": os.path.basename(inf), "verdict": "TLE",
                    "input_preview": preview_text(inp_data, 800),
                    "expected_preview": preview_text(exp_data, 800),
                    "actual_preview": f"Quá thời gian ({int(tl_sec * 1000)} ms)",
                }, 200

            if u_res.returncode != 0:
                return {
                    "status": "FOUND", "iter": it, "seed": os.path.basename(inf), "verdict": "RE",
                    "input_preview": preview_text(inp_data, 800),
                    "expected_preview": preview_text(exp_data, 800),
                    "actual_preview": preview_text(u_res.stderr or f"Exit code {u_res.returncode}", 800),
                }, 200

            ok = False
            checker_msg = ""
            if has_checker:
                exp_p = os.path.join(tmpdir, "st.exp")
                act_p = os.path.join(tmpdir, "st.act")
                open(exp_p, "w", encoding="utf-8").write(exp_data)
                open(act_p, "w", encoding="utf-8").write(u_res.stdout)
                chk = subprocess.run(
                    [sys.executable, checker_path, inf, exp_p, act_p],
                    capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=10,
                )
                ok = (chk.returncode == 0)
                checker_msg = (chk.stdout or chk.stderr or "").strip()
            else:
                ok = (normalize_output(u_res.stdout) == normalize_output(exp_data))

            if not ok:
                return {
                    "status": "FOUND", "iter": it, "seed": os.path.basename(inf), "verdict": "WA",
                    "checker_msg": checker_msg,
                    "input_preview": preview_text(inp_data, 800),
                    "expected_preview": preview_text(exp_data, 800),
                    "actual_preview": preview_text(u_res.stdout, 800),
                }, 200
            tested = it

    return {"status": "PASSED", "iters": tested}, 200


class TTUDHandler(BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        pass  # Keep console clean

    def send_json(self, obj, status=200):
        raw = json.dumps(obj, ensure_ascii=False).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(raw)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(raw)

    def read_json_body(self):
        length = int(self.headers.get("Content-Length", "0"))
        if length <= 0 or length > 2_000_000:
            return None
        raw = self.rfile.read(length).decode("utf-8", errors="replace")
        return json.loads(raw)

    def do_GET(self):
        parsed = urlparse(self.path)
        path = parsed.path

        if path == "/api/bootstrap":
            with open(os.path.join(DATA_DIR, "roadmap.json"), "r", encoding="utf-8") as f:
                roadmap = json.load(f)
            with open(os.path.join(DATA_DIR, "theory.json"), "r", encoding="utf-8") as f:
                theory = json.load(f)
            # Merge bai giang chi tiet (data/lessons/<chapId>.json) vao tung chuong
            lessons_dir = os.path.join(DATA_DIR, "lessons")
            for chap in theory.get("chapters", []):
                lp = os.path.join(lessons_dir, chap.get("id", "") + ".json")
                if os.path.isfile(lp):
                    try:
                        with open(lp, "r", encoding="utf-8") as f:
                            chap["lesson"] = json.load(f)
                    except Exception as e:
                        print(f"[WARN] Loi doc {lp}: {e}")
            rec_path = os.path.join(DATA_DIR, "recognition.json")
            if os.path.isfile(rec_path):
                with open(rec_path, "r", encoding="utf-8") as f:
                    theory["recognition"] = json.load(f)
            mock_exams_path = os.path.join(DATA_DIR, "mock_exams.json")
            mock_exams = {}
            if os.path.isfile(mock_exams_path):
                with open(mock_exams_path, "r", encoding="utf-8") as f:
                    mock_exams = json.load(f)
            practice_path = os.path.join(DATA_DIR, "practice_8days.json")
            practice_8days = {}
            if os.path.isfile(practice_path):
                with open(practice_path, "r", encoding="utf-8") as f:
                    practice_8days = json.load(f)
            problems = load_all_problem_summaries()
            for p in problems:
                p["has_guide"] = os.path.isfile(os.path.join(PROBLEMS_DIR, p["id"], "guide.json"))

            conn = get_db()
            cur = conn.cursor()
            cur.execute("SELECT * FROM problem_status")
            status_map = {r["problem_id"]: dict(r) for r in cur.fetchall()}
            cur.execute("SELECT day, checked FROM roadmap_checked")
            roadmap_checked = {str(r["day"]): r["checked"] for r in cur.fetchall()}
            cur.execute("SELECT * FROM mistakes")
            mistakes_map = {r["problem_id"]: dict(r) for r in cur.fetchall()}
            cur.execute("SELECT key, value FROM settings")
            settings = {r["key"]: r["value"] for r in cur.fetchall()}
            cur.execute("SELECT * FROM exam_history ORDER BY id DESC LIMIT 30")
            exam_history = []
            for r in cur.fetchall():
                d = dict(r)
                try:
                    d["problems"] = json.loads(d.pop("problems_json") or "[]")
                except Exception:
                    d["problems"] = []
                exam_history.append(d)
            conn.close()

            for p in problems:
                p["status"] = status_map.get(p["id"], {
                    "solved": 0, "attempts": 0, "best_time_ms": None, "last_verdict": "", "last_submitted_at": None
                })
            return self.send_json({
                "roadmap": roadmap,
                "practice_8days": practice_8days,
                "theory": theory,
                "mock_exams": mock_exams,
                "exam_history": exam_history,
                "problems": problems,
                "roadmap_checked": roadmap_checked,
                "mistakes": mistakes_map,
                "settings": settings,
                "stats": compute_stats(problems, status_map),
                "gxx_path": GXX_PATH,
                "gdb_path": debugger.GDB_PATH or "",
                "debugger_backend": debugger.ACTIVE_BACKEND or "",
                "debugger_path": debugger.ACTIVE_DEBUGGER_PATH or "",
            })

        if path.startswith("/api/problem/"):
            parts = path[len("/api/problem/"):].split("/")
            pid = parts[0]
            pdir = safe_pid(pid)
            if not pdir:
                return self.send_json({"error": "Not found"}, 404)

            if len(parts) == 2 and parts[1] == "test-preview":
                qs = parse_qs(parsed.query)
                tname = qs.get("name", [""])[0]
                if not re.match(r"^[a-zA-Z0-9._-]+$", tname):
                    return self.send_json({"error": "Invalid test name"}, 400)
                tdir = os.path.join(pdir, "tests")
                inp_p = os.path.join(tdir, tname + ".in")
                out_p = os.path.join(tdir, tname + ".out")
                if not os.path.isfile(inp_p):
                    return self.send_json({"error": "Test not found"}, 404)
                inp = open(inp_p, "r", encoding="utf-8", errors="replace").read(4096)
                out = open(out_p, "r", encoding="utf-8", errors="replace").read(4096) if os.path.isfile(out_p) else ""
                return self.send_json({
                    "name": tname,
                    "input": inp,
                    "output": out,
                    "input_size": os.path.getsize(inp_p),
                    "output_size": os.path.getsize(out_p) if os.path.isfile(out_p) else 0,
                })

            with open(os.path.join(pdir, "meta.json"), "r", encoding="utf-8") as f:
                meta = json.load(f)
            ref_path = os.path.join(pdir, "ref.cpp")
            ref_cpp = open(ref_path, "r", encoding="utf-8").read() if os.path.isfile(ref_path) else ""
            guide_path = os.path.join(pdir, "guide.json")
            if os.path.isfile(guide_path):
                try:
                    with open(guide_path, "r", encoding="utf-8") as f:
                        meta["guide"] = json.load(f)
                except Exception as e:
                    print(f"[WARN] Loi doc {guide_path}: {e}")
            tests_dir = os.path.join(pdir, "tests")
            test_names = sorted([os.path.splitext(f)[0] for f in os.listdir(tests_dir) if f.endswith(".in")]) if os.path.isdir(tests_dir) else []

            conn = get_db()
            cur = conn.cursor()
            cur.execute("SELECT content FROM notes WHERE problem_id = ?", (pid,))
            nrow = cur.fetchone()
            cur.execute("SELECT code FROM drafts WHERE problem_id = ?", (pid,))
            drow = cur.fetchone()
            cur.execute("""
                SELECT id, verdict, passed, total, max_time_ms, compile_error, details_json, code, created_at
                FROM submissions WHERE problem_id = ? ORDER BY id DESC LIMIT 20
            """, (pid,))
            subs = []
            for r in cur.fetchall():
                d = dict(r)
                try:
                    d["details"] = json.loads(d.pop("details_json") or "[]")
                except Exception:
                    d["details"] = []
                subs.append(d)
            cur.execute("SELECT * FROM problem_status WHERE problem_id = ?", (pid,))
            st = cur.fetchone()
            conn.close()

            meta["ref_cpp"] = ref_cpp
            meta["tests_dir"] = tests_dir
            meta["test_names"] = test_names
            meta["note"] = nrow["content"] if nrow else ""
            meta["draft"] = drow["code"] if drow else ""
            meta["submissions"] = subs
            meta["status"] = dict(st) if st else {"solved": 0, "attempts": 0, "best_time_ms": None, "last_verdict": ""}
            return self.send_json(meta)

        if path.startswith("/api/lecture/"):
            fname = os.path.basename(path[len("/api/lecture/"):])
            if not re.match(r"^lec\d+\.pdf$", fname):
                return self.send_json({"error": "Invalid filename"}, 400)
            fpath = os.path.join(LECTURE_PDF_DIR, fname)
            if not os.path.isfile(fpath):
                return self.send_json({"error": "Lecture PDF not found"}, 404)
            with open(fpath, "rb") as f:
                data = f.read()
            self.send_response(200)
            self.send_header("Content-Type", "application/pdf")
            self.send_header("Content-Length", str(len(data)))
            self.end_headers()
            self.wfile.write(data)
            return

        # Serve static files
        rel = path.lstrip("/")
        if rel == "":
            rel = "index.html"
        fpath = os.path.normpath(os.path.join(STATIC_DIR, rel))
        if not fpath.startswith(os.path.normpath(STATIC_DIR)) or not os.path.isfile(fpath):
            self.send_error(404, "File not found")
            return
        ctype, _ = mimetypes.guess_type(fpath)
        if not ctype:
            ctype = "application/octet-stream"
        if ctype.startswith("text/") or ctype == "application/javascript":
            ctype += "; charset=utf-8"
        with open(fpath, "rb") as f:
            data = f.read()
        self.send_response(200)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_POST(self):
        path = urlparse(self.path).path
        try:
            body = self.read_json_body()
        except Exception as e:
            return self.send_json({"error": f"Bad JSON: {e}"}, 400)
        if body is None:
            return self.send_json({"error": "Empty or oversized body"}, 400)

        if path == "/api/run-sample":
            res, code = judge_run_sample(
                body.get("problem_id", ""),
                body.get("code", ""),
                body.get("custom_input", ""),
            )
            return self.send_json(res, code)

        if path == "/api/submit":
            res, code = judge_submit(
                body.get("problem_id", ""),
                body.get("code", ""),
            )
            return self.send_json(res, code)

        if path == "/api/stress-test":
            res, code = judge_stress_test(
                body.get("problem_id", ""),
                body.get("code", ""),
                body.get("max_iters", 60),
            )
            return self.send_json(res, code)

        if path == "/api/mistake":
            pid = body.get("problem_id", "")
            if not safe_pid(pid):
                return self.send_json({"error": "Invalid problem"}, 400)
            tag = str(body.get("tag", ""))[:80]
            note = str(body.get("note", ""))[:1000]
            resolved = 1 if body.get("resolved") else 0
            ts = now_iso()
            conn = get_db()
            conn.execute(
                """
                INSERT INTO mistakes (problem_id, last_verdict, tag, note, fail_count, resolved, updated_at)
                VALUES (?, '', ?, ?, 1, ?, ?)
                ON CONFLICT(problem_id) DO UPDATE SET
                    tag = excluded.tag,
                    note = excluded.note,
                    resolved = excluded.resolved,
                    updated_at = excluded.updated_at
                """,
                (pid, tag, note, resolved, ts),
            )
            conn.commit()
            cur = conn.cursor()
            cur.execute("SELECT * FROM mistakes WHERE problem_id = ?", (pid,))
            rec = dict(cur.fetchone())
            conn.close()
            return self.send_json({"ok": True, "mistake": rec})

        if path == "/api/notes":
            pid = body.get("problem_id", "")
            if not safe_pid(pid):
                return self.send_json({"error": "Invalid problem"}, 400)
            conn = get_db()
            conn.execute(
                """
                INSERT INTO notes (problem_id, content, updated_at) VALUES (?, ?, ?)
                ON CONFLICT(problem_id) DO UPDATE SET content = excluded.content, updated_at = excluded.updated_at
                """,
                (pid, body.get("content", ""), now_iso()),
            )
            conn.commit()
            conn.close()
            return self.send_json({"ok": True})

        if path == "/api/draft":
            pid = body.get("problem_id", "")
            if not safe_pid(pid):
                return self.send_json({"error": "Invalid problem"}, 400)
            conn = get_db()
            conn.execute(
                """
                INSERT INTO drafts (problem_id, code, updated_at) VALUES (?, ?, ?)
                ON CONFLICT(problem_id) DO UPDATE SET code = excluded.code, updated_at = excluded.updated_at
                """,
                (pid, body.get("code", ""), now_iso()),
            )
            conn.commit()
            conn.close()
            return self.send_json({"ok": True})

        if path == "/api/roadmap/toggle":
            try:
                day = int(body.get("day", 0))
            except (ValueError, TypeError):
                return self.send_json({"error": "Invalid day"}, 400)
            checked = 1 if body.get("checked") else 0
            conn = get_db()
            conn.execute(
                """
                INSERT INTO roadmap_checked (day, checked, updated_at) VALUES (?, ?, ?)
                ON CONFLICT(day) DO UPDATE SET checked = excluded.checked, updated_at = excluded.updated_at
                """,
                (day, checked, now_iso()),
            )
            conn.commit()
            conn.close()
            return self.send_json({"ok": True, "day": day, "checked": checked})

        if path == "/api/settings":
            key = str(body.get("key", ""))
            value = str(body.get("value", ""))
            if not re.match(r"^[a-zA-Z0-9_-]{1,40}$", key):
                return self.send_json({"error": "Invalid key"}, 400)
            conn = get_db()
            conn.execute(
                """
                INSERT INTO settings (key, value) VALUES (?, ?)
                ON CONFLICT(key) DO UPDATE SET value = excluded.value
                """,
                (key, value),
            )
            conn.commit()
            conn.close()
            return self.send_json({"ok": True})

        if path == "/api/exam/finish":
            exam_id = str(body.get("exam_id", "custom"))
            title = str(body.get("title", "Đề thi thử"))
            track = str(body.get("track", "midterm"))
            try:
                total_score = float(body.get("total_score", 0.0))
                duration_used_sec = int(body.get("duration_used_sec", 0))
            except (ValueError, TypeError):
                return self.send_json({"error": "Invalid numeric score or duration"}, 400)
            problems = body.get("problems", [])
            feedback = str(body.get("feedback", ""))
            ts = now_iso()
            conn = get_db()
            cur = conn.cursor()
            cur.execute(
                """
                INSERT INTO exam_history (exam_id, title, track, total_score, duration_used_sec, problems_json, feedback, created_at)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?)
                """,
                (exam_id, title, track, total_score, duration_used_sec, json.dumps(problems, ensure_ascii=False), feedback, ts),
            )
            new_id = cur.lastrowid
            conn.commit()
            conn.close()
            return self.send_json({
                "ok": True,
                "record": {
                    "id": new_id,
                    "exam_id": exam_id,
                    "title": title,
                    "track": track,
                    "total_score": total_score,
                    "duration_used_sec": duration_used_sec,
                    "problems": problems,
                    "feedback": feedback,
                    "created_at": ts,
                }
            })

        if path == "/api/debug/start":
            res = DEBUG_MANAGER.start(
                body.get("code", ""),
                body.get("stdin", ""),
                body.get("breakpoints", []),
                body.get("watches", []),
                prev_session_id=body.get("session_id"),
            )
            return self.send_json(res)

        if path == "/api/debug/action":
            res = DEBUG_MANAGER.action(
                body.get("session_id", ""),
                body.get("action", ""),
                line=body.get("line"),
                level=body.get("level"),
            )
            return self.send_json(res)

        if path == "/api/debug/breakpoints":
            res = DEBUG_MANAGER.sync_breakpoints(
                body.get("session_id", ""),
                body.get("breakpoints", []),
            )
            return self.send_json(res)

        if path == "/api/debug/watches":
            res = DEBUG_MANAGER.set_watches(
                body.get("session_id", ""),
                body.get("watches", []),
            )
            return self.send_json(res)

        if path == "/api/debug/stop":
            res = DEBUG_MANAGER.stop(body.get("session_id", ""))
            return self.send_json(res)

        return self.send_json({"error": "Not found"}, 404)


def get_lan_ips():
    ips = []
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
            s.connect(("8.8.8.8", 80))
            ip = s.getsockname()[0]
            if ip and not ip.startswith("127."):
                ips.append(ip)
    except OSError:
        pass
    try:
        for info in socket.getaddrinfo(socket.gethostname(), None, socket.AF_INET):
            ip = info[4][0]
            if ip and not ip.startswith("127.") and ip not in ips:
                ips.append(ip)
    except OSError:
        pass
    return ips


def main():
    init_db()
    host = os.environ.get("HOST", "0.0.0.0")
    port = int(os.environ.get("PORT", sys.argv[1] if len(sys.argv) > 1 else 8765))
    server = ThreadingHTTPServer((host, port), TTUDHandler)
    print(f"TTUD Local Judge & Learning App running at http://127.0.0.1:{port}")
    if host == "0.0.0.0":
        for ip in get_lan_ips():
            print(f"LAN / Network access: http://{ip}:{port}")
    print(f"C++ Compiler: {GXX_PATH}")
    dbg_info = f"{debugger.ACTIVE_BACKEND} ({debugger.ACTIVE_DEBUGGER_PATH})" if debugger.ACTIVE_BACKEND else "Not found"
    print(f"C++ Debugger: {dbg_info}")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down...")
        server.server_close()


if __name__ == "__main__":
    main()
