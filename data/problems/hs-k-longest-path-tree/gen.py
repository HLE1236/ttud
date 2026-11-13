import os, sys, random
tdir = sys.argv[1]
random.seed(20250311)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "12 4\n1 2 3\n1 3 6\n1 4 2\n2 5 4\n2 10 7\n3 6 5\n3 7 2\n3 8 9\n4 9 3\n7 11 7\n7 12 3")
w(2, "4 10\n1 2 5\n2 3 6\n3 4 7")
for idx in range(3, 11):
    n = random.choice([20, 200, 1000, 5000, 10000])
    K = random.randint(2, min(n - 1, random.choice([5, 20, 60, 100])))
    lines = [f"{n} {K}"]
    for v in range(2, n + 1):
        lines.append(f"{random.randint(1, v - 1)} {v} {random.randint(1, 1000)}")
    w(idx, "\n".join(lines))
