import os, sys, random
tdir = sys.argv[1]
random.seed(20250206)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "6 3\n-2 -7 2 5 2 4")
w(2, "4 5\n1 1 1 1")
w(3, "5 7\n-1 -2 -3 -4 -5")
for idx in range(4, 11):
    n = random.choice([50, 500, 5000, 50000, 100000])
    K = random.randint(2, 10)
    arr = [random.randint(-1000, 1000) for _ in range(n)]
    w(idx, f"{n} {K}\n" + " ".join(map(str, arr)))
