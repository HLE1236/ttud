import sys, os, random
tdir = sys.argv[1]; rng = random.Random(20201)
cases = ["5\n5 8 7 1 9\n4\n1 2\n1 5\n1 3\n4 5\n"]
for n, q in [(20, 20), (100, 100), (1000, 1000), (10000, 10000), (50000, 50000), (100000, 100000), (100000, 100000), (100000, 100000), (100000, 100000)]:
    lines = [str(n), " ".join(str(rng.randint(1, 1000)) for _ in range(n)), str(q)]
    for _ in range(q):
        i = rng.randint(1, n); j = rng.randint(i, n)
        lines.append(f"{i} {j}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
