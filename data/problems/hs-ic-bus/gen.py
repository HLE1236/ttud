import os, sys, random
tdir = sys.argv[1]
random.seed(20250317)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "6 6\n10 2\n30 1\n50 1\n20 3\n30 1\n20 1\n1 2\n1 3\n1 5\n2 4\n2 5\n4 6")
for idx in range(2, 11):
    n = random.choice([20, 100, 500, 2000, 5000])
    m = min(10000, n * 2)
    lines = [f"{n} {m}"]
    for _ in range(n):
        lines.append(f"{random.randint(1, 10000)} {random.randint(1, min(100, max(2, n // 3)))}")
    edges = [(i, i + 1) for i in range(1, n)]
    while len(edges) < m:
        edges.append((random.randint(1, n), random.randint(1, n)))
    random.shuffle(edges)
    lines.extend(f"{u} {v}" for u, v in edges[:m])
    w(idx, "\n".join(lines))
