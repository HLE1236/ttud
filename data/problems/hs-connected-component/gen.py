import os, sys, random
tdir = sys.argv[1]
random.seed(20250303)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "8 8\n1 2\n1 7\n1 8\n2 7\n4 5\n4 6\n5 6\n7 8")
w(2, "10 0")
for idx in range(3, 11):
    n = random.choice([30, 500, 5000, 30000, 100000])
    m = random.randint(0, min(100000, n * 2))
    lines = [f"{n} {m}"] + [f"{random.randint(1, n)} {random.randint(1, n)}" for _ in range(m)]
    w(idx, "\n".join(lines))
