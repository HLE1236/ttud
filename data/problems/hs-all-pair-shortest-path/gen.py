import sys, os, random
random.seed(504)
tdir = sys.argv[1]
cases = [
    "4 9\n1 2 9\n1 3 7\n1 4 2\n2 1 1\n2 4 5\n3 4 6\n3 2 2\n4 1 5\n4 2 8\n",
    "3 1\n1 2 5\n",
]
for idx in range(8):
    n = 15 if idx < 3 else (80 if idx < 6 else 250)
    m = min(n * (n - 1), random.randint(n, n * 6))
    lines = [f"{n} {m}"]
    for _ in range(m):
        u = random.randint(1, n)
        v = random.randint(1, n)
        while v == u and n > 1:
            v = random.randint(1, n)
        lines.append(f"{u} {v} {random.randint(1, 1000)}")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
