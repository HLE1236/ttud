import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202513)
cases = [
    "3 3\n1 1\n3 3\n5 2 4\n3 0 7\n6 3 8\n",
    "1 1\n1 1\n1 1\n42\n",
    "2 2\n1 1\n2 2\n1 0\n0 5\n",
    "3 4\n1 1\n3 4\n1 2 3 4\n5 6 7 8\n9 10 11 12\n",
]
for M, N, p0 in [(3, 5, 0.2), (4, 4, 0.15), (4, 5, 0.25), (4, 5, 0.1), (5, 4, 0.2), (5, 5, 0.3)]:
    lines = [f"{M} {N}", "1 1", f"{M} {N}"]
    for i in range(M):
        row = []
        for j in range(N):
            if (i, j) not in [(0,0), (M-1, N-1)] and rng.random() < p0:
                row.append("0")
            else:
                row.append(str(rng.randint(1, 100)))
        lines.append(" ".join(row))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
