import os, sys, random
tdir = sys.argv[1]
random.seed(20250315)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "5 8\n1 2 1\n1 3 4\n1 5 1\n2 4 2\n2 5 1\n3 4 3\n3 5 3\n4 5 2")
for idx in range(2, 11):
    n = random.choice([20, 300, 3000, 30000, 100000])
    m = min(100000, n * 2)
    edges = [(random.randint(1, i - 1), i, random.randint(1, 10000)) for i in range(2, n + 1)]
    while len(edges) < m:
        edges.append((random.randint(1, n), random.randint(1, n), random.randint(1, 10000)))
    w(idx, f"{n} {len(edges)}\n" + "\n".join(f"{u} {v} {wt}" for u, v, wt in edges))
