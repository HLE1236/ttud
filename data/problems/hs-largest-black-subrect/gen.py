import sys, os, random
random.seed(403)
tdir = sys.argv[1]
cases = [
    "4 4\n0 1 1 1\n1 1 1 0\n1 1 0 0\n1 1 1 1\n",
    "3 3\n0 0 0\n0 0 0\n0 0 0\n",
    "3 3\n1 1 1\n1 1 1\n1 1 1\n",
]
for idx in range(7):
    n = 20 if idx < 3 else (200 if idx < 5 else 800)
    m = 20 if idx < 3 else (200 if idx < 5 else 800)
    p = 0.85 if idx % 2 == 0 else 0.6
    lines = [f"{n} {m}"]
    for _ in range(n):
        lines.append(" ".join("1" if random.random() < p else "0" for _ in range(m)))
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
