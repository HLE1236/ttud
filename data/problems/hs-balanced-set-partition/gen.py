import os, sys, random
tdir = sys.argv[1]
random.seed(20250214)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "10 3\n6 5 5 7 3 6 1 4 9 3")
w(2, "6 2\n10 10 10 10 10 10")
for idx in range(3, 11):
    n = random.choice([20, 100, 500, 2000, 5000])
    m = random.randint(2, min(n, random.choice([5, 20, 100])))
    ws = [random.randint(1, 10000) for _ in range(n)]
    w(idx, f"{n} {m}\n" + " ".join(map(str, ws)))
