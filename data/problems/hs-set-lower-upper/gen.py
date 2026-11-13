import sys, os, random
tdir = sys.argv[1]; rng = random.Random(10105)
cases = ["5\n3 2 4 5 9\nmin_greater_equal 10\nmin_greater 7\nmin_greater_equal 7\nmin_greater_equal 4\ninsert 5\ninsert 5\nmin_greater 8\nmin_greater 1\nmin_greater_equal 6\nmin_greater_equal 8\n#\n"]
cmds = ["min_greater_equal", "min_greater", "insert", "remove"]
for n, q, mx in [(10, 20, 50), (100, 200, 1000), (1000, 2000, 10000), (10000, 10000, 50000), (50000, 50000, 100000), (100000, 50000, 100000), (100000, 80000, 100000), (100000, 100000, 100000), (100000, 100000, 200000)]:
    lines = [str(n), " ".join(str(rng.randint(1, mx)) for _ in range(n))]
    for _ in range(q):
        lines.append(f"{rng.choice(cmds)} {rng.randint(1, mx)}")
    lines.append("#")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
