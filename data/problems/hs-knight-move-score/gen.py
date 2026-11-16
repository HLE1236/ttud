import sys, os, random
random.seed(209)
tdir = sys.argv[1]
cases = [
    "4 1 1\n3 4 8 7\n0 3 0 0\n2 0 -10 0\n0 -4 0 10\n",
    "1 1 1\n-5\n",
    "2 1 1\n5 -3\n2 4\n",
    "3 2 2\n1 2 3\n4 5 6\n7 8 9\n",
]
for idx in range(6):
    n = 4 if idx < 4 else 5
    sr = random.randint(1, n)
    sc = random.randint(1, n)
    lines = [f"{n} {sr} {sc}"]
    for _ in range(n):
        lines.append(" ".join(str(random.randint(-10, 15)) for _ in range(n)))
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
