import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202224)
cases = [
    "3\n1 2 3\n",
    "5\n1 2 3 4 5\n",
    "4\n10 10 10 30\n",
    "6\n100 100 100 100 100 100\n",
]
for sz, lo, hi in [(8, 10, 100), (12, 1000, 5000), (15, 1000, 10000), (17, 1000, 50000), (18, 1000, 100000), (19, 1000, 100000)]:
    arr = [str(rng.randint(lo, hi)) for _ in range(sz)]
    cases.append(f"{sz}\n" + " ".join(arr) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
