import sys, os, random
tdir = sys.argv[1]; rng = random.Random(20202)
cases = ["3 4\n1 1 0 1\n1 1 1 0\n0 1 0 0\n3\n2 3 3 4\n1 3 3 4\n2 2 3 4\n"]
for n, m, q in [(10, 10, 20), (50, 50, 100), (100, 100, 1000), (200, 300, 5000), (400, 400, 10000), (500, 500, 20000), (700, 700, 50000), (900, 900, 80000), (1000, 1000, 100000)]:
    lines = [f"{n} {m}"]
    for _ in range(n): lines.append(" ".join(str(rng.randint(0, 100)) for _ in range(m)))
    lines.append(str(q))
    for _ in range(q):
        r1 = rng.randint(1, n); r2 = rng.randint(r1, n)
        c1 = rng.randint(1, m); c2 = rng.randint(c1, m)
        lines.append(f"{r1} {c1} {r2} {c2}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
