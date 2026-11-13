import sys, os, random
tdir = sys.argv[1]; rng = random.Random(30701)
cases = ["6\n0 10\n3 7\n6 14\n9 11\n12 15\n17 19\n"]
for n, mx in [(10, 50), (100, 500), (1000, 5000), (5000, 50000), (20000, 200000), (50000, 500000), (80000, 1000000), (100000, 1000000), (100000, 100000)]:
    lines = [str(n)]
    for _ in range(n):
        a = rng.randint(0, mx - 1); b = rng.randint(a + 1, min(mx, a + 200))
        lines.append(f"{a} {b}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
