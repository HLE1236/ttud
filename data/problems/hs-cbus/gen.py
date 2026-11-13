import sys, os, random
tdir = sys.argv[1]; rng = random.Random(30402)
cases = ["3 2\n0 8 5 1 10 5 9\n9 0 5 6 6 2 8\n2 2 0 3 8 7 2\n5 3 4 0 3 2 7\n9 6 8 7 0 9 10\n3 8 10 6 5 0 2\n3 4 4 5 2 2 0\n"]
for n, cap in [(2, 1), (4, 2), (5, 2), (6, 3), (7, 2), (8, 3), (9, 4), (10, 3), (11, 4)]:
    sz = 2 * n + 1
    lines = [f"{n} {cap}"]
    for i in range(sz):
        lines.append(" ".join("0" if i == j else str(rng.randint(1, 25)) for j in range(sz)))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
