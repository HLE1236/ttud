import sys, os, random
tdir = sys.argv[1]; rng = random.Random(40602)
cases = ["6\n3 10\n2 40\n6 70\n3 50\n5 80\n1 60\n"]
for n in [10, 50, 200, 1000, 5000, 20000, 50000, 80000, 100000]:
    lines = [str(n)] + [f"{rng.randint(1, n)} {rng.randint(1, 100000)}" for _ in range(n)]
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
