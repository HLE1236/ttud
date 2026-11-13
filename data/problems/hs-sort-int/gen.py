import sys, os, random
tdir = sys.argv[1]; rng = random.Random(40801)
cases = ["4\n1 3 4 2\n", "1\n100\n"]
for n in [20, 200, 2000, 10000, 50000, 100000, 200000, 400000]:
    cases.append(f"{n}\n" + " ".join(str(rng.randint(0, 100000)) for _ in range(n)) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
