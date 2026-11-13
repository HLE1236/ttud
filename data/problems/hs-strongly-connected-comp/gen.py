import os, sys, random
tdir = sys.argv[1]
random.seed(20250307)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "8 13\n1 2\n1 8\n2 3\n2 6\n3 6\n4 3\n4 5\n5 4\n6 4\n7 1\n7 2\n7 6\n8 7")
w(2, "4 4\n1 2\n2 3\n3 4\n4 1")
for idx in range(3, 11):
    n = random.choice([30, 500, 5000, 30000, 60000])
    m = min(200000, n * 3)
    lines = [f"{n} {m}"] + [f"{random.randint(1, n)} {random.randint(1, n)}" for _ in range(m)]
    w(idx, "\n".join(lines))
