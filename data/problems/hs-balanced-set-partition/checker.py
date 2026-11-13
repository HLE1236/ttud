import sys
in_p, exp_p, out_p = sys.argv[1], sys.argv[2], sys.argv[3]
itok = list(map(int, open(in_p).read().split()))
n, m = itok[0], itok[1]
w = itok[2:2+n]

def eval_file(path):
    toks = list(map(int, open(path).read().split()))
    if len(toks) != n + 1 or toks[0] != n:
        raise ValueError(f"Expected n={n} followed by {n} integers")
    x = toks[1:]
    loads = [0] * m
    for i, b in enumerate(x):
        if not (1 <= b <= m):
            raise ValueError(f"Bin index {b} out of range 1..{m}")
        loads[b - 1] += w[i]
    return max(loads) - min(loads)

try:
    exp_diff = eval_file(exp_p)
    act_diff = eval_file(out_p)
except Exception as e:
    print(f"Format/constraint error: {e}")
    sys.exit(1)

limit = max(exp_diff + 5, int(exp_diff * 1.35))
if act_diff > limit:
    print(f"Imbalance too large: got diff={act_diff}, expected <= {limit} (ref={exp_diff})")
    sys.exit(1)
sys.exit(0)
