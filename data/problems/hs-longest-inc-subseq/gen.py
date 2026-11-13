import os, sys, random
tdir = sys.argv[1]
random.seed(20250203)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "6\n5 8 2 8 10 10")
w(2, "5\n5 4 3 2 1")
w(3, "5\n1 2 3 4 5")
w(4, "1\n42")
for idx in range(5, 11):
    n = random.choice([50, 500, 5000, 50000, 100000])
    arr = [random.randint(-10**6, 10**6) for _ in range(n)]
    w(idx, f"{n}\n" + " ".join(map(str, arr)))
