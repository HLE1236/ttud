import sys, os, random
tdir = sys.argv[1]; rng = random.Random(20203)
cases = ["5 6\n5 2 1 4 3\n", "4 10\n1 2 3 4\n"]
for n, mx in [(20, 50), (100, 300), (1000, 3000), (10000, 30000), (50000, 200000), (100000, 500000), (200000, 1000000), (300000, 1000000)]:
    arr = rng.sample(range(1, mx + 1), n)
    M = arr[0] + arr[-1]
    cases.append(f"{n} {M}\n" + " ".join(map(str, arr)) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
