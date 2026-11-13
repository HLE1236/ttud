import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2025102)
cases = [
    "5 1\n1 2 3 4 5\n2 3\n",
    "1 1\n1000000000\n1 1\n",
    "5 3\n-5 10 -3 4 -1\n1 5\n2 2\n4 2\n",
]
for n, q, lo, hi in [(20, 15, -100, 100), (1000, 500, -10**6, 10**6), (10000, 5000, -10**9, 10**9), (100000, 20000, 0, 10**9), (200000, 50000, -10**9, 10**9), (300000, 50000, -1000, 1000), (500000, 100000, 1, 10**9)]:
    arr = [str(rng.randint(lo, hi)) for _ in range(n)]
    lines = [f"{n} {q}", " ".join(arr)]
    for _ in range(q):
        i = rng.randint(1, n)
        k = rng.randint(1, n - i + 1)
        lines.append(f"{i} {k}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
