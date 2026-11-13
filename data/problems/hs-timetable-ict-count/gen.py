import os, sys, random
tdir = sys.argv[1]
random.seed(20250211)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "2\nIT3011 3 100\nIT3020 4 120\n1\nD9-101 150 6 3 3 6 2 11 1 16 5 23 3 26 5")
w(2, "6\nIT3011 3 100\nIT3020 4 120\nIT4663 3 60\nIT3220 4 90\nIT3030 3 100\nIT3170 2 120\n4\nD9-101 150 6 3 3 6 2 11 1 16 5 23 3 26 5\nC7-205 80 9 1 2 5 1 6 1 10 1 11 5 16 1 20 1 21 3 29 2\nD5-301 120 7 1 1 4 2 6 3 11 5 16 2 21 2 26 5\nD8-102 100 7 1 2 5 1 6 3 11 3 16 5 21 1 26 5")
w(3, "2\nA 5 100\nB 5 100\n1\nR1 90 1 1 6")
for idx in range(4, 11):
    n = random.randint(3, 8)
    m = random.randint(2, 5)
    lines = [str(n)]
    for i in range(n):
        lines.append(f"C{i+1} {random.randint(2, 4)} {random.randint(40, 120)}")
    lines.append(str(m))
    for j in range(m):
        k = random.randint(2, 5)
        cap = random.randint(60, 160)
        cur = 1
        pers = []
        for _ in range(k):
            l = random.randint(2, 5)
            pers.append(f"{cur} {l}")
            cur += l + 1
        lines.append(f"R{j+1} {cap} {k} " + " ".join(pers))
    w(idx, "\n".join(lines))
