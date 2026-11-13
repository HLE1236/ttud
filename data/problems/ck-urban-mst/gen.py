import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2023233)
cases = [
    "4\n3 4\n4 1\n4 3\n1 1\n1\n4 2\n",
    "2\n0 0\n3 4\n0\n",
    "3\n0 0\n10 0\n0 10\n2\n1 2\n2 3\n",
]
for n, m, cmax in [(10, 3, 50), (30, 10, 200), (80, 20, 1000), (150, 50, 5000), (300, 100, 10000), (500, 200, 10000), (750, 500, 10000)]:
    lines = [str(n)]
    for _ in range(n):
        lines.append(f"{rng.randint(-cmax, cmax)} {rng.randint(-cmax, cmax)}")
    lines.append(str(m))
    for _ in range(m):
        u = rng.randint(1, n); v = rng.randint(1, n)
        lines.append(f"{u} {v}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
