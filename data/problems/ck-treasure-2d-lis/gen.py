import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2023112)
cases = [
    "10\n2 4 7\n4 2 10\n4 6 2\n5 5 7\n1 3 9\n1 5 1\n1 1 3\n3 1 5\n3 3 6\n2 2 2\n",
    "1\n0 0 100\n",
    "4\n1 1 5\n1 1 7\n1 2 3\n2 2 4\n",
]
for n, xy_max, c_max in [(15, 20, 50), (50, 100, 1000), (150, 500, 10000), (300, 1000, 10**6), (500, 10000, 10**9), (800, 10**6, 10**9), (1000, 10**9, 10**9)]:
    lines = [str(n)]
    for _ in range(n):
        lines.append(f"{rng.randint(0, xy_max)} {rng.randint(0, xy_max)} {rng.randint(1, c_max)}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
