import os, sys, random
tdir = sys.argv[1]
random.seed(20250316)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "4 4 7\n1 2 1\n1 3 2\n2 4 4\n3 4 3")
for idx in range(2, 11):
    n = random.randint(4, 10)
    m = random.randint(n - 1, min(18, n * (n - 1) // 2))
    edges = [(random.randint(1, i - 1), i, random.randint(1, 15)) for i in range(2, n + 1)]
    seen = set((min(u, v), max(u, v)) for u, v, _ in edges)
    while len(edges) < m:
        u = random.randint(1, n); v = random.randint(1, n)
        if u != v and (min(u, v), max(u, v)) not in seen:
            seen.add((min(u, v), max(u, v)))
            edges.append((u, v, random.randint(1, 15)))
    K = random.randint(n * 2, min(100, n * 10))
    w(idx, f"{n} {len(edges)} {K}\n" + "\n".join(f"{u} {v} {wt}" for u, v, wt in edges))
