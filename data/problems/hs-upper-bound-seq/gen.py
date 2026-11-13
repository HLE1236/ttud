import sys, os, random
tdir = sys.argv[1]; rng = random.Random(10104)
cases = ["5\n4 8 2 3 7\nnext 2\nnext 7\nnext 9\nnext 6\nnext 1\n#\n"]
for n, q, mx in [(10, 20, 50), (100, 100, 1000), (1000, 1000, 10000), (10000, 10000, 100000), (50000, 50000, 1000000), (80000, 80000, 1000000), (100000, 100000, 1000000), (100000, 100000, 500), (100000, 100000, 1000000)]:
    lines = [str(n), " ".join(str(rng.randint(1, mx)) for _ in range(n))]
    for _ in range(q): lines.append(f"next {rng.randint(0, mx + 5)}")
    lines.append("#")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
