import os, sys, random
tdir = sys.argv[1]
random.seed(20250319)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "7 12\n6 7\n1 7 7\n2 3 6\n2 5 6\n3 1 6\n3 7 11\n4 1 7\n4 2 4\n4 5 5\n5 1 4\n5 3 4\n6 2 8\n6 4 10")
for idx in range(2, 11):
    n = random.choice([20, 100, 500, 2000, 5000])
    m = min(50000, n * 4)
    s, t = 1, n
    lines = [f"{n} {m}", f"{s} {t}"]
    for _ in range(m):
        u = random.randint(1, n); v = random.randint(1, n)
        if u == v: v = (u % n) + 1
        lines.append(f"{u} {v} {random.randint(1, 1000)}")
    w(idx, "\n".join(lines))
