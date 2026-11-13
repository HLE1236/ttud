import sys, os, random
tdir = sys.argv[1]; rng = random.Random(30704)
cases = ["6 6\n7 3 10 1 2 8\n6 2 8 10 5 7\n"]
for n, mx in [(20, 50), (100, 300), (1000, 3000), (5000, 20000), (20000, 100000), (50000, 200000), (80000, 500000), (100000, 1000000), (100000, 200000)]:
    a = rng.sample(range(1, mx + 1), n); b = rng.sample(range(1, mx + 1), n)
    cases.append(f"{n} {n}\n" + " ".join(map(str, a)) + "\n" + " ".join(map(str, b)) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
