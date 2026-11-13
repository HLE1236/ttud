import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202221)
cases = [
    "2\n11\n1 3 5 8 9 2 6 7 6 8 9\n11\n1 1 1 1 1 1 1 1 1 1 1\n",
    "3\n1\n5\n2\n0 3\n5\n2 1 0 0 4\n",
    "1\n6\n5 1 1 1 1 0\n",
]
for tc, n, lo, hi in [(3, 30, 1, 5), (5, 500, 1, 20), (4, 5000, 0, 15), (3, 50000, 1, 50), (2, 200000, 1, 100), (2, 300000, 1, 10), (1, 500000, 1, 100)]:
    lines = [str(tc)]
    for _ in range(tc):
        lines.append(str(n))
        arr = [str(rng.randint(lo, hi)) for _ in range(n)]
        lines.append(" ".join(arr))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
