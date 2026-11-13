import os, sys, random
tdir = sys.argv[1]
random.seed(20250301)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "6 7\n2 4\n1 3\n3 4\n5 6\n1 2\n3 5\n2 3")
w(2, "5 0")
for idx in range(3, 11):
    n = random.choice([20, 200, 2000, 20000, 50000])
    m = min(n * 3, random.randint(n // 2, n * 2))
    edges = set()
    while len(edges) < m:
        u = random.randint(1, n); v = random.randint(1, n)
        if u != v: edges.add((min(u, v), max(u, v)))
    lines = [f"{n} {len(edges)}"] + [f"{u} {v}" for u, v in edges]
    w(idx, "\n".join(lines))
