import sys, os, random
random.seed(506)
tdir = sys.argv[1]
cases = [
    "4 4\n8\n1 2 1\n1 4 7\n2 1 1\n2 3 2\n3 2 6\n3 3 9\n4 3 8\n4 4 9\n",
    "2 3\n3\n1 1 5\n1 2 10\n2 2 6\n",
]
for idx in range(8):
    n = 15 if idx < 3 else (60 if idx < 6 else 150)
    m = 15 if idx < 3 else (60 if idx < 6 else 150)
    K = min(n * m, random.randint(n, n * 10))
    lines = [f"{n} {m}", str(K)]
    for _ in range(K):
        lines.append(f"{random.randint(1, n)} {random.randint(1, m)} {random.randint(1, 10000)}")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
