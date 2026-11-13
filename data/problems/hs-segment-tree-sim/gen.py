import sys, os, random
tdir = sys.argv[1]; rng = random.Random(20302)
cases = ["10\n1 10 9 7 1 4 2 4 8 10\n5\nget-max 5 8\nget-max 5 9\nget-max 3 8\nupdate 9 20\nget-max 4 10\n"]
for n, m in [(20, 20), (100, 100), (1000, 1000), (5000, 5000), (20000, 20000), (50000, 50000), (80000, 80000), (100000, 100000), (100000, 100000)]:
    lines = [str(n), " ".join(str(rng.randint(1, 10000)) for _ in range(n)), str(m)]
    for _ in range(m):
        if rng.random() < 0.4:
            lines.append(f"update {rng.randint(1, n)} {rng.randint(1, 100000)}")
        else:
            l = rng.randint(1, n); r = rng.randint(l, n)
            lines.append(f"get-max {l} {r}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
