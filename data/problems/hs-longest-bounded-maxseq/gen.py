import sys, os, random
tdir = sys.argv[1]; rng = random.Random(20204)
cases = ["10 40\n19 2 7 6 9 1 17 19 8 4\n", "5 3\n10 10 10 10 10\n"]
for n, Q, mx in [(20, 50, 15), (200, 300, 30), (2000, 1000, 50), (20000, 5000, 100), (100000, 20000, 500), (200000, 50000, 1000), (300000, 100000, 2000), (500000, 100000, 5000)]:
    cases.append(f"{n} {Q}\n" + " ".join(str(rng.randint(1, mx)) for _ in range(n)) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
