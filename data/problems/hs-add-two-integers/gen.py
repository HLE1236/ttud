import sys, os, random
tdir = sys.argv[1]; rng = random.Random(10101)
cases = ["3 5\n", "0 0\n", "9999999999999999999 1\n", "10000000000000000000 10000000000000000000\n"]
for _ in range(6):
    cases.append(f"{rng.randint(0, 10**19)} {rng.randint(0, 10**19)}\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
