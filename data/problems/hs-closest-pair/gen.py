import os, sys, random
tdir = sys.argv[1]
random.seed(20250210)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "5\n0 0\n1 1\n100 45\n2 3\n9 9")
w(2, "2\n-1000000 -1000000\n1000000 1000000")
for idx in range(3, 11):
    n = random.choice([50, 500, 5000, 50000, 100000])
    pts = set()
    while len(pts) < n:
        pts.add((random.randint(-1000000, 1000000), random.randint(-1000000, 1000000)))
    pts = list(pts)
    lines = [str(n)] + [f"{x} {y}" for x, y in pts]
    w(idx, "\n".join(lines))
