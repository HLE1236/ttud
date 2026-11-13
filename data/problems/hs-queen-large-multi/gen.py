import os, sys, random
tdir = sys.argv[1]
random.seed(20250213)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
cases = [(10, 2), (10, 5), (12, 6), (20, 5), (30, 8), (50, 5), (80, 5), (120, 4), (200, 3), (300, 2)]
for idx, (n, k) in enumerate(cases, 1):
    w(idx, f"{n} {k}")
