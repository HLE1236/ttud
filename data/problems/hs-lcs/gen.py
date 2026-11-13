import os, sys, random
tdir = sys.argv[1]
random.seed(20250202)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "7 10\n3 7 2 5 1 4 9\n4 3 2 3 6 1 5 4 9 7")
w(2, "5 5\n1 2 3 4 5\n6 7 8 9 10")
w(3, "5 5\n1 2 3 4 5\n1 2 3 4 5")
for idx in range(4, 11):
    n = random.choice([30, 200, 1000, 3000, 4000])
    m = random.choice([30, 200, 1000, 3000, 4000])
    A = [random.randint(1, 30) for _ in range(n)]
    B = [random.randint(1, 30) for _ in range(m)]
    w(idx, f"{n} {m}\n" + " ".join(map(str, A)) + "\n" + " ".join(map(str, B)))
