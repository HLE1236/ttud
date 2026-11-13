import os, sys, random
tdir = sys.argv[1]
random.seed(20250324)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "10 2 1 1 10")
w(2, "10 2 2 1 10")
w(3, "100 1 1 50 50")
for idx in range(4, 11):
    n = random.choice([50, 500, 10000, 50000, 100000])
    a = random.randint(1, max(2, n // 10))
    b = random.randint(1, max(2, n // 10))
    X = random.randint(1, n)
    Y = random.randint(1, n)
    w(idx, f"{n} {a} {b} {X} {Y}")
