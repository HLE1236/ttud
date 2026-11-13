import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2023151)
cases = [
    "6 10\n2 1 6 7 4 5\n",
    "3 6\n1 2 3\n",
    "4 100\n1 2 3 4\n",
]
for n, hi in [(20, 50), (100, 300), (500, 2000), (1000, 5000), (2500, 20000), (4000, 100000), (5000, 500000)]:
    arr = rng.sample(range(1, hi + 1), n)
    Q = sum(rng.sample(arr, 3))
    cases.append(f"{n} {Q}\n" + " ".join(map(str, arr)) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
