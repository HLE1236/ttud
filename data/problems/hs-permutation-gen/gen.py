import sys, os
tdir = sys.argv[1]
cases = [3, 1, 2, 4, 5, 6, 7, 8, 4, 5]
for i, v in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(f"{v}\n")
