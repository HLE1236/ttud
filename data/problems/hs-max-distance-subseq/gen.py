import sys, os, random
tdir = sys.argv[1]; rng = random.Random(40702)
cases = ["1\n5 3\n1 2 8 4 9\n", "2\n2 2\n0 1000000000\n5 5\n1 2 3 4 5\n"]
for tc, N, mx in [(2, 10, 50), (3, 100, 1000), (4, 1000, 100000), (5, 5000, 10**6), (4, 20000, 10**9), (3, 50000, 10**9), (2, 100000, 10**9), (2, 100000, 10**9)]:
    lines = [str(tc)]
    for _ in range(tc):
        C = rng.randint(2, N)
        lines.append(f"{N} {C}")
        lines.append(" ".join(str(rng.randint(0, mx)) for _ in range(N)))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
