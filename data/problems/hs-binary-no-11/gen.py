import sys, os
tdir = sys.argv[1]
cases = [f"{k}\n" for k in [3, 1, 2, 4, 5, 7, 10, 13, 16, 18]]
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
