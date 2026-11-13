import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202121)
cases = [
    "7\n1 3 2 4 6 8 5\n",
    "1\n10\n",
    "5\n5 4 3 2 1\n",
    "5\n2 2 2 2 2\n",
    "6\n1 2 3 4 5 6\n",
]
for sz, lo, hi in [(30, -20, 20), (1000, -10**6, 10**6), (50000, 0, 100), (200000, -10**9, 10**9), (500000, 1, 1000000)]:
    arr = [str(rng.randint(lo, hi)) for _ in range(sz)]
    cases.append(f"{sz}\n" + " ".join(arr) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
