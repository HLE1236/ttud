import sys
in_p, exp_p, out_p = sys.argv[1], sys.argv[2], sys.argv[3]
itok = list(map(int, open(in_p).read().split()))
n = itok[0]
mat = [itok[1 + i*n : 1 + (i+1)*n] for i in range(n)]

def eval_tour(path):
    toks = list(map(int, open(path).read().split()))
    if len(toks) != n + 1 or toks[0] != n:
        raise ValueError(f"Expected n={n} followed by {n} cities")
    tour = toks[1:]
    if sorted(tour) != list(range(1, n + 1)):
        raise ValueError("Not a valid permutation of 1..n")
    cost = 0
    for i in range(n):
        u = tour[i] - 1
        v = tour[(i + 1) % n] - 1
        cost += mat[u][v]
    return cost

try:
    exp_cost = eval_tour(exp_p)
    act_cost = eval_tour(out_p)
except Exception as e:
    print(f"Invalid tour: {e}")
    sys.exit(1)

limit = int(exp_cost * 1.25) + (20 if n <= 10 else 5)
if act_cost > limit:
    print(f"Tour cost too large: got {act_cost}, limit {limit} (ref={exp_cost})")
    sys.exit(1)
sys.exit(0)
