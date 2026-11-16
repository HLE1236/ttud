import sys
in_p, exp_p, out_p = sys.argv[1], sys.argv[2], sys.argv[3]
lines_in = open(in_p, encoding="utf-8").read().strip().splitlines()
N, M, K = map(int, lines_in[0].split())
orig_cap = {}
for i in range(1, 1 + M):
    u, v, c = map(int, lines_in[i].split())
    orig_cap[(u, v)] = orig_cap.get((u, v), 0) + c
reqs = [tuple(map(int, lines_in[1 + M + i].split())) for i in range(K)]

def check_out(path):
    lines = [l.strip() for l in open(path, encoding="utf-8").read().splitlines() if l.strip() != ""]
    if len(lines) != K:
        raise ValueError(f"Expected {K} lines, got {len(lines)}")
    used = {}
    cnt = 0
    for i, line in enumerate(lines):
        toks = list(map(int, line.split()))
        if toks == [-1]:
            continue
        s, d = reqs[i]
        if len(toks) < 2 or toks[0] != s or toks[-1] != d:
            raise ValueError(f"Line {i+1}: path does not start at {s} and end at {d}")
        for j in range(len(toks) - 1):
            e = (toks[j], toks[j + 1])
            if orig_cap.get(e, 0) <= 0:
                raise ValueError(f"Line {i+1}: edge {e} does not exist")
            used[e] = used.get(e, 0) + 1
            if used[e] > orig_cap[e]:
                raise ValueError(f"Edge {e} exceeded capacity {orig_cap[e]}")
        cnt += 1
    return cnt

try:
    exp_cnt = check_out(exp_p)
    act_cnt = check_out(out_p)
except Exception as e:
    print(f"Invalid output: {e}")
    sys.exit(1)

if act_cnt < int(exp_cnt * 0.9):
    print(f"Too few conveyors: got {act_cnt}, expected >= {int(exp_cnt * 0.9)} (ref={exp_cnt})")
    sys.exit(1)
sys.exit(0)
