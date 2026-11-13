import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202511)
cases = []
cases.append("5 120\n1 120 121 20 60\n")
cases.append("1 0\n0\n")
cases.append("1 -1000000000\n1000000000\n")
cases.append("6 100\n100 100 100 100 100 100\n")
cases.append("8 -5\n-10 -5 -4 0 -6 10 -5 1\n")
for sz, lo, hi in [(50, -100, 100), (1000, -10**9, 10**9), (50000, -1000, 1000), (200000, -10**9, 10**9), (500000, 0, 500)]:
    c = rng.randint(lo, hi)
    arr = [str(rng.randint(lo, hi)) for _ in range(sz)]
    cases.append(f"{sz} {c}\n" + " ".join(arr) + "\n")
for i, content in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{i:02d}.in"), "w", encoding="utf-8") as f:
        f.write(content)
