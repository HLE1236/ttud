import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2025122)
cases = [
    "10 3\n0 0 0 1 0 0 0 0 1 0\n",
    "5 1\n0 1 0 1 0\n",
    "5 5\n0 0 0 0 0\n",
    "5 2\n1 1 1 1 1\n",
    "1 1\n0\n",
]
for L, k, p0 in [(30, 4, 0.7), (1000, 5, 0.8), (50000, 10, 0.9), (200000, 3, 0.6), (500000, 100, 0.95)]:
    arr = ["0" if rng.random() < p0 else "1" for _ in range(L)]
    cases.append(f"{L} {k}\n" + " ".join(arr) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
