import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2024142)
cases = [
    "4 3\n1 2\n2 2\n3 1\n4 2\n",
    "5 1\n1 10\n2 20\n3 30\n4 40\n5 50\n",
    "4 4\n10 5\n20 5\n30 5\n40 5\n",
    "5 2\n1 1\n2 1\n3 1\n4 1\n5 10\n",
]
for n, K, hi in [(10, 3, 20), (20, 4, 50), (40, 5, 100), (60, 8, 500), (80, 10, 1000), (100, 15, 2000)]:
    lines = [f"{n} {K}"]
    vals = sorted(rng.sample(range(1, 10000), n))
    for v in vals:
        lines.append(f"{v} {rng.randint(1, hi)}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
