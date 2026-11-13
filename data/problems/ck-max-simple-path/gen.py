import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2025105)
cases = [
    "5 6\n1 2 10\n2 3 20\n3 4 15\n4 5 5\n1 3 3\n2 5 8\n",
    "4 0\n",
    "4 4\n1 2 5\n2 3 5\n3 4 5\n4 1 5\n",
]
for n, p, wmax in [(6, 0.7, 50), (8, 0.6, 100), (10, 0.5, 200), (12, 0.5, 500), (13, 0.45, 1000), (14, 0.4, 10000), (15, 0.35, 100000)]:
    edges = []
    for u in range(1, n + 1):
        for v in range(u + 1, n + 1):
            if rng.random() < p:
                edges.append(f"{u} {v} {rng.randint(1, wmax)}")
    lines = [f"{n} {len(edges)}"] + edges
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
