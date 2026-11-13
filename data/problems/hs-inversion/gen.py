import sys, os, random
tdir = sys.argv[1]; rng = random.Random(40701)
cases = ["6\n3 2 4 5 6 1\n", "5\n5 4 3 2 1\n", "5\n1 2 3 4 5\n"]
for n in [30, 300, 3000, 20000, 100000, 200000, 400000]:
    cases.append(f"{n}\n" + " ".join(str(rng.randint(0, 10**6)) for _ in range(n)) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
