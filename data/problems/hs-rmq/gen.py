import sys, os, random
tdir = sys.argv[1]; rng = random.Random(20301)
cases = ["16\n2 4 6 1 6 8 7 3 3 5 8 9 1 2 6 4\n4\n1 5\n0 9\n1 15\n6 10\n"]
for n, m in [(20, 15), (200, 100), (2000, 1000), (10000, 10000), (50000, 50000), (100000, 100000), (200000, 200000), (300000, 300000), (500000, 500000)]:
    lines = [str(n), " ".join(str(rng.randint(1, 10**6)) for _ in range(n)), str(m)]
    for _ in range(m):
        i = rng.randrange(n); j = rng.randint(i, n - 1)
        lines.append(f"{i} {j}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
