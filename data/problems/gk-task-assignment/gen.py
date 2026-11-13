import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202413)
cases = [
    "3 5 2\n1 1 1 1 1\n3 2 6 1 2\n2 2 3 1 5\n",
    "2 5 2\n1 2 3 4 5\n5 4 3 2 1\n",
    "1 4 4\n5 10 15 20\n",
    "2 4 2\n10 10 10 10\n20 20 20 20\n",
]
for m, n, D, hi in [(3, 8, 3, 20), (4, 10, 3, 50), (5, 12, 3, 100), (6, 15, 3, 100), (8, 16, 2, 200), (10, 18, 2, 300)]:
    lines = [f"{m} {n} {D}"]
    for _ in range(m):
        lines.append(" ".join(str(rng.randint(1, hi)) for _ in range(n)))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
