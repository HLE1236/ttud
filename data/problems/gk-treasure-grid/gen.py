import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2023133)
cases = [
    "5 6\n1 2 0 5 1 3\n5 -9 2 0 1 3\n1 4 1 5 7 4\n-60 1 0 0 0 0\n6 7 0 5 4 3\n",
    "5 6\n1 2 0 5 1 3\n5 -9 2 0 1 3\n1 4 1 5 7 4\n-60 1 9 0 29 0\n6 7 0 5 4 3\n",
    "1 1\n-15\n",
    "3 3\n0 1 2\n3 4 5\n6 7 8\n",
]
for n, m, p0 in [(10, 12, 0.2), (30, 30, 0.25), (100, 100, 0.15), (300, 300, 0.35), (500, 500, 0.1), (800, 800, 0.1)]:
    lines = [f"{n} {m}"]
    for i in range(n):
        row = []
        for j in range(m):
            if (i, j) not in [(0, 0), (n-1, m-1)] and rng.random() < p0:
                row.append("0")
            else:
                v = rng.randint(-100, 100)
                if v == 0: v = 1
                row.append(str(v))
        lines.append(" ".join(row))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
