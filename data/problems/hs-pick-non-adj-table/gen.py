import os, sys, random
tdir = sys.argv[1]
random.seed(20250209)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "2 4\n2 1 30 8\n9 4 6 5")
w(2, "1 5\n5 1 1 5 5")
w(3, "4 1\n10\n20\n15\n5")
for idx in range(4, 11):
    m = random.choice([4, 6, 8, 10])
    n = random.choice([20, 100, 500, 1000])
    lines = [f"{m} {n}"]
    for _ in range(m):
        lines.append(" ".join(str(random.randint(1, 1000)) for _ in range(n)))
    w(idx, "\n".join(lines))
