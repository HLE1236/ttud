import os, sys, random
tdir = sys.argv[1]
random.seed(20250312)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "5 8\n1 2 8\n2 3 8\n3 4 4\n4 5 10\n1 4 3\n2 4 9\n1 7 1\n5 6 9")
w(2, "4 2\n1 2 5\n3 4 10")
for idx in range(3, 11):
    n = random.choice([20, 300, 3000, 30000, 100000])
    m = min(200000, n * 3)
    edges = [(i, i + 1, random.randint(1, 1000)) for i in range(1, n)]
    while len(edges) < m:
        u = random.randint(1, n - 1)
        v = random.randint(u + 1, n)
        edges.append((u, v, random.randint(1, 1000)))
    w(idx, f"{n} {len(edges)}\n" + "\n".join(f"{u} {v} {wt}" for u, v, wt in edges))
