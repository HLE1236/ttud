import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202414)
cases = [
    "6 2\n3 2 4 5 7 6\n",
    "1 5\n10\n",
    "5 10\n1 5 9 13 17\n",
    "6 3\n1 4 7 10 13 16\n",
]
for n, D, hi in [(20, 5, 50), (100, 10, 500), (500, 20, 5000), (2000, 100, 50000), (5000, 500, 500000), (10000, 1000, 1000000)]:
    arr = [str(rng.randint(1, hi)) for _ in range(n)]
    cases.append(f"{n} {D}\n" + " ".join(arr) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
