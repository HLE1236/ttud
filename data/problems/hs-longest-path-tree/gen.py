import os, sys, random
tdir = sys.argv[1]
random.seed(20250308)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "6\n1 3 3\n1 6 2\n2 6 5\n4 5 2\n4 6 1")
w(2, "1")
for idx in range(3, 11):
    n = random.choice([20, 300, 3000, 30000, 80000])
    lines = [str(n)]
    for v in range(2, n + 1):
        u = random.randint(1, v - 1)
        lines.append(f"{u} {v} {random.randint(1, 100)}")
    w(idx, "\n".join(lines))
