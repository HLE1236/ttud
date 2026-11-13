import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2025103)
cases = [
    "3 2\n2 3\n1 2\n",
    "4 4\n1 2\n2 3\n3 1\n3 4\n",
    "4 0\n",
    "5 10\n" + "\n".join(f"{i} {j}" for i in range(1, 6) for j in range(i+1, 6)) + "\n",
]
for n, p in [(6, 0.4), (8, 0.5), (10, 0.3), (12, 0.4), (14, 0.5), (16, 0.6)]:
    edges = [(i, j) for i in range(1, n+1) for j in range(i+1, n+1) if rng.random() < p]
    lines = [f"{n} {len(edges)}"] + [f"{u} {v}" for u, v in edges]
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
