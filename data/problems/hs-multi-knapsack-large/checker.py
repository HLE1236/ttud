import sys
in_p, exp_p, out_p = sys.argv[1], sys.argv[2], sys.argv[3]
itok = list(map(int, open(in_p).read().split()))
m, n, P = itok[0], itok[1], itok[2]
r = itok[3:3+n]
trucks = [(itok[3+n+2*j], itok[3+n+2*j+1]) for j in range(m)]

def eval_sol(path):
    toks = list(map(int, open(path).read().split()))
    if len(toks) != n + 1 or toks[0] != n:
        raise ValueError(f"Expected n={n} followed by {n} assignments")
    s = toks[1:]
    loads = [0] * m
    used = [False] * m
    rev = 0
    for i, t in enumerate(s):
        if t == -1: continue
        if not (1 <= t <= m):
            raise ValueError(f"Invalid truck {t}")
        used[t - 1] = True
        loads[t - 1] += r[i]
        rev += P * r[i]
    cost = 0
    for j in range(m):
        if loads[j] > trucks[j][0]:
            raise ValueError(f"Truck {j+1} overloaded: {loads[j]} > {trucks[j][0]}")
        if used[j]:
            cost += trucks[j][1]
    return rev - cost

try:
    exp_profit = eval_sol(exp_p)
    act_profit = eval_sol(out_p)
except Exception as e:
    print(f"Invalid output: {e}")
    sys.exit(1)

min_req = int(exp_profit * 0.85)
if act_profit < min_req:
    print(f"Profit too low: got {act_profit}, need >= {min_req} (ref={exp_profit})")
    sys.exit(1)
sys.exit(0)
