import sys, os, random
tdir = sys.argv[1]
rng = random.Random(20241245)
cases = [
    "6 2\n10 22 5 75 65 80\n",
    "8 3\n12 14 17 10 14 13 12 15\n",
    "5 1\n90 80 70 60 50\n",
    "2 1\n10 100\n",
]
for n, k, hi in [(20, 4, 100), (200, 10, 1000), (1000, 50, 100000), (10000, 100, 10**6), (50000, 1000, 10**9), (100000, 50, 10**9)]:
    arr = [str(rng.randint(1, hi)) for _ in range(n)]
    cases.append(f"{n} {k}\n" + " ".join(arr) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
