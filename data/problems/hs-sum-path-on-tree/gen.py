import os, sys, random
tdir = sys.argv[1]
random.seed(20250310)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "4\n1 2 1\n1 3 3\n3 4 2")
w(2, "2\n1 2 100")
for idx in range(3, 11):
    n = random.choice([20, 300, 3000, 30000, 80000])
    lines = [str(n)]
    for v in range(2, n + 1):
        lines.append(f"{random.randint(1, v - 1)} {v} {random.randint(1, 1000)}")
    w(idx, "\n".join(lines))
