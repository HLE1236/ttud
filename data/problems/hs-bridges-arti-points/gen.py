import os, sys, random
tdir = sys.argv[1]
random.seed(20250306)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "10 12\n1 10\n10 2\n10 3\n2 4\n4 5\n5 2\n3 6\n6 7\n7 3\n7 8\n8 9\n9 7")
w(2, "4 3\n1 2\n2 3\n3 4")
for idx in range(3, 11):
    n = random.choice([20, 200, 1000, 10000, 30000])
    m = min(60000, n + random.randint(n // 4, n))
    edges = [(random.randint(1, i - 1), i) for i in range(2, n + 1)]
    while len(edges) < m:
        u = random.randint(1, n); v = random.randint(1, n)
        if u != v: edges.append((u, v))
    w(idx, f"{n} {len(edges)}\n" + "\n".join(f"{u} {v}" for u, v in edges))
