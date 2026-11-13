import os, sys, random
tdir = sys.argv[1]
random.seed(20250313)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "9 13\n5 3 1 2 6 4 3 1 4\n1 3\n1 5\n1 6\n2 1\n2 3\n3 5\n4 1\n4 2\n4 6\n5 8\n7 9\n9 5\n9 8")
for idx in range(2, 11):
    n = random.choice([20, 200, 1000, 5000, 10000])
    perm = list(range(1, n + 1)); random.shuffle(perm)
    ds = [random.randint(1, 1000) for _ in range(n)]
    m = min(100000, n * 4)
    edges = set()
    while len(edges) < m:
        i = random.randint(0, n - 2); j = random.randint(i + 1, n - 1)
        edges.add((perm[i], perm[j]))
    lines = [f"{n} {len(edges)}", " ".join(map(str, ds))] + [f"{u} {v}" for u, v in edges]
    w(idx, "\n".join(lines))
