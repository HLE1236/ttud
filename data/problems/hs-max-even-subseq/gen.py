import os, sys, random
tdir = sys.argv[1]
random.seed(20250204)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "4\n1 2 -3 4")
w(2, "1\n7")
w(3, "3\n-2 -4 -6")
w(4, "4\n-1 -3 -5 -7")
for idx in range(5, 11):
    n = random.choice([100, 1000, 50000, 200000, 500000])
    arr = [random.randint(-10**6, 10**6) for _ in range(n)]
    w(idx, f"{n}\n" + " ".join(map(str, arr)))
