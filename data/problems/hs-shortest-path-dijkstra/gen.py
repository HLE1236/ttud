import os, sys, random
tdir = sys.argv[1]
random.seed(20250314)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "5 7\n2 5 87\n1 2 97\n4 5 78\n3 1 72\n1 4 19\n2 3 63\n5 1 18\n1 5")
w(2, "4 2\n1 2 10\n3 4 10\n1 4")
for idx in range(3, 11):
    n = random.choice([30, 500, 5000, 30000, 80000])
    m = min(100000, n * 3)
    lines = [f"{n} {m}"] + [f"{random.randint(1, n)} {random.randint(1, n)} {random.randint(1, 1000)}" for _ in range(m)]
    lines.append(f"{random.randint(1, n)} {random.randint(1, n)}")
    w(idx, "\n".join(lines))
