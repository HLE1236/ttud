import sys, os, random
tdir = sys.argv[1]; rng = random.Random(40601)
cases = ["5\n3 7 4 9 1\n", "4\n1 1 1 1\n"]
for n, mx in [(10, 20), (100, 100), (1000, 500), (5000, 5000), (20000, 100000), (50000, 100000), (100000, 1000000), (100000, 50000)]:
    cases.append(f"{n}\n" + " ".join(str(rng.randint(1, mx)) for _ in range(n)) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
