import os, sys, random
tdir = sys.argv[1]
random.seed(20250205)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "6 2 3")
w(2, "2 1 2")
w(3, "5 3 4")
w(4, "10 2 5")
for idx, tc in enumerate([(50, 3, 7), (100, 5, 15), (300, 10, 40), (700, 20, 100), (1000, 5, 400), (1000, 300, 400)], 5):
    w(idx, f"{tc[0]} {tc[1]} {tc[2]}")
