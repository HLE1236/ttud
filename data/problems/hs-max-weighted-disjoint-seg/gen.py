import os, sys, random
tdir = sys.argv[1]
random.seed(20250207)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "3\n2 100\n1 1000\n900 1010")
w(2, "7\n1 4\n3 16\n5 9\n7 13\n10 19\n14 26\n20 24")
w(3, "3\n1 5\n5 10\n10 15")
for idx in range(4, 11):
    n = random.choice([20, 200, 1000, 5000, 10000])
    lines = [str(n)]
    for _ in range(n):
        a = random.randint(0, 99000)
        b = random.randint(a, min(100000, a + random.randint(1, 5000)))
        lines.append(f"{a} {b}")
    w(idx, "\n".join(lines))
