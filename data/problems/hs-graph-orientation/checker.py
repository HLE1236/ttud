import sys
from collections import Counter
in_p, exp_p, out_p = sys.argv[1], sys.argv[2], sys.argv[3]
itok = list(map(int, open(in_p).read().split()))
n, m = itok[0], itok[1]
inp_edges = Counter()
for i in range(m):
    u, v = itok[2 + 2*i], itok[3 + 2*i]
    inp_edges[(min(u, v), max(u, v))] += 1

exp_toks = list(map(int, open(exp_p).read().split()))
out_toks = list(map(int, open(out_p).read().split()))
if not out_toks:
    print("Empty output"); sys.exit(1)
if exp_toks[0] == -1:
    if out_toks != [-1]:
        print(f"Expected -1, got {out_toks[:5]}"); sys.exit(1)
    sys.exit(0)

if out_toks[0] != m or len(out_toks) != 1 + 2 * m:
    print(f"Expected m={m} followed by {2*m} ints")
    sys.exit(1)

adj = [[] for _ in range(n + 1)]
radj = [[] for _ in range(n + 1)]
used = Counter()
for i in range(m):
    u, v = out_toks[1 + 2*i], out_toks[2 + 2*i]
    if not (1 <= u <= n and 1 <= v <= n):
        print("Invalid vertex index"); sys.exit(1)
    used[(min(u, v), max(u, v))] += 1
    adj[u].append(v); radj[v].append(u)

if used != inp_edges:
    print("Directed edges do not match input undirected edges")
    sys.exit(1)

# Check strong connectivity with 2 iterative BFS/DFS
def reach_all(g):
    vis = [False] * (n + 1)
    vis[1] = True; st = [1]; cnt = 0
    while st:
        u = st.pop(); cnt += 1
        for v in g[u]:
            if not vis[v]: vis[v] = True; st.append(v)
    return cnt == n

if not (reach_all(adj) and reach_all(radj)):
    print("Oriented graph is not strongly connected")
    sys.exit(1)
sys.exit(0)
