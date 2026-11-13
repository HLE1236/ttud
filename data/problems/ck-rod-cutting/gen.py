import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2023234)
cases = [
    "8\n1 5 8 9 10 17 17 20\n",
    "8\n3 5 8 9 10 17 17 20\n",
    "1\n1000000000\n",
    "5\n1 2 3 4 5\n",
]
for n, hi in [(20, 100), (100, 1000), (300, 100000), (500, 10**6), (800, 10**9), (1000, 10**9)]:
    arr = [str(rng.randint(1, hi)) for _ in range(n)]
    cases.append(f"{n}\n" + " ".join(arr) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
