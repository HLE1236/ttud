import sys, os, random
tdir = sys.argv[1]; rng = random.Random(30501)
cases = ["4 2 15\n7 7 11 2\n0 12 12 11 14\n14 0 11 14 14\n14 10 0 11 12\n10 14 12 0 13\n10 13 14 11 0\n"]
for n, K, Q in [(4, 2, 18), (6, 2, 22), (7, 3, 20), (8, 3, 25), (9, 3, 25), (10, 4, 24), (11, 4, 25), (12, 4, 28), (12, 5, 25)]:
    dem = [rng.randint(3, 9) for _ in range(n)]
    lines = [f"{n} {K} {Q}", " ".join(map(str, dem))]
    for i in range(n + 1):
        lines.append(" ".join("0" if i == j else str(rng.randint(5, 30)) for j in range(n + 1)))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
