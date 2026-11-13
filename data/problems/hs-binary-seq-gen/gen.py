import sys, os
tdir = sys.argv[1]
cases = [f"{k}\n" for k in [3, 1, 2, 4, 5, 6, 8, 10, 12, 14]]
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
