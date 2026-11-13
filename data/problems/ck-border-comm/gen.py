import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2023243)
cases = [
    "2 4\n0 1\n2 3\n0 6\n1 7\n",
    "1 3\n0 0\n3 4\n0 10\n",
    "2 3\n0 0\n1 0\n100 100\n",
]
for S, P, hi in [(1, 10, 50), (3, 25, 200), (5, 60, 1000), (10, 150, 5000), (20, 300, 10000), (15, 400, 10000), (50, 500, 10000)]:
    lines = [f"{S} {P}"]
    for _ in range(P):
        lines.append(f"{rng.randint(-hi, hi)} {rng.randint(-hi, hi)}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
