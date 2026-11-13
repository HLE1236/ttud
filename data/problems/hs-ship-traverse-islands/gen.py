import os, sys, random
tdir = sys.argv[1]
random.seed(20250323)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "8 4\n6 0\n3 1\n10 2\n7 3\n9 4\n10 7\n2 6\n7 9")
w(2, "3 2\n0 0\n1 1\n100 100")
for idx in range(3, 11):
    n = random.choice([20, 200, 1000, 5000, 10000])
    D = random.randint(200, 1500)
    lines = [f"{n} {D}"] + [f"{random.randint(0, 10000)} {random.randint(0, 10000)}" for _ in range(n)]
    w(idx, "\n".join(lines))
