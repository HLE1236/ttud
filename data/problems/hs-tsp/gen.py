import sys, os, random
tdir = sys.argv[1]; rng = random.Random(40901)
cases = ["4\n0 1 1 9\n1 0 9 3\n1 9 0 2\n9 3 2 0\n"]
for n in [5, 7, 9, 11, 13, 15, 16, 17, 18]:
    lines = [str(n)]
    for i in range(n):
        lines.append(" ".join("0" if i == j else str(rng.randint(1, 50)) for j in range(n)))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
