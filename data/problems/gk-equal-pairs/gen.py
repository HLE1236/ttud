import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202311)
cases = [
    "6\n1 2 2 1 3 1\n",
    "1\n5\n",
    "5\n1 2 3 4 5\n",
    "6\n1000000 1000000 1000000 1000000 1000000 1000000\n",
]
for sz, mx in [(20, 5), (500, 50), (5000, 1000), (50000, 100), (100000, 3), (100000, 1000000)]:
    arr = [str(rng.randint(1, mx)) for _ in range(sz)]
    cases.append(f"{sz}\n" + " ".join(arr) + "\n")
for i, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{i:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
