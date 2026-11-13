import os, sys, random
tdir = sys.argv[1]
random.seed(20250215)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "3 6 20\n4 2 5 7 3 1\n10 8\n12 9\n15 10")
for idx in range(2, 11):
    m = random.choice([5, 15, 40, 100, 200])
    n = random.choice([20, 60, 200, 500, 1000])
    P = random.randint(10, 50)
    rs = [random.randint(1, 30) for _ in range(n)]
    lines = [f"{m} {n} {P}", " ".join(map(str, rs))]
    for _ in range(m):
        q = random.randint(25, 120)
        c = random.randint(50, P * 35)
        lines.append(f"{q} {c}")
    w(idx, "\n".join(lines))
