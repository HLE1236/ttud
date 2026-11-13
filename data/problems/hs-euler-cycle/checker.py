import sys
from collections import Counter
in_p, exp_p, out_p = sys.argv[1], sys.argv[2], sys.argv[3]
itok = list(map(int, open(in_p).read().split()))
n, m = itok[0], itok[1]
edges = Counter()
for i in range(m):
    u, v = itok[2 + 2*i], itok[3 + 2*i]
    edges[(min(u, v), max(u, v))] += 1

exp_toks = list(map(int, open(exp_p).read().split()))
out_toks = list(map(int, open(out_p).read().split()))
if not out_toks:
    print("Empty output"); sys.exit(1)
if exp_toks[0] == -1:
    if out_toks != [-1]:
        print(f"Expected -1, got {out_toks[:5]}"); sys.exit(1)
    sys.exit(0)

L = out_toks[0]
if L != m + 1 or len(out_toks) != L + 1:
    print(f"Expected length {m+1} + sequence, got L={L}, tokens={len(out_toks)}")
    sys.exit(1)
path = out_toks[1:]
if path[0] != path[-1]:
    print("Cycle does not start and end at same vertex")
    sys.exit(1)
used = Counter()
for i in range(m):
    u, v = path[i], path[i+1]
    if not (1 <= u <= n and 1 <= v <= n):
        print("Invalid vertex in path"); sys.exit(1)
    used[(min(u, v), max(u, v))] += 1
if used != edges:
    print("Used edges do not match input edges exactly")
    sys.exit(1)
sys.exit(0)
