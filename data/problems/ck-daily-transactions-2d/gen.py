import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202422)
cases = [
    "3\n1 1 100\n1 2 200\n2 1 300\n2\n1 1 2\n2 1 1\n",
    "1\n10000 100 1000000\n2\n10000 1 100\n9999 1 100\n",
]
for n, m, maxd in [(20, 15, 10), (200, 100, 50), (2000, 1000, 500), (10000, 10000, 2000), (50000, 50000, 5000), (100000, 100000, 10000), (200000, 200000, 10000), (300000, 300000, 10000)]:
    lines = [str(n)]
    for _ in range(n):
        lines.append(f"{rng.randint(1, maxd)} {rng.randint(1, 100)} {rng.randint(1, 1000000)}")
    lines.append(str(m))
    for _ in range(m):
        c1 = rng.randint(1, 100)
        c2 = rng.randint(c1, 100)
        lines.append(f"{rng.randint(1, maxd)} {c1} {c2}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
