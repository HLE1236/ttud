import sys, os
tdir = sys.argv[1]
cases = [4, 1, 2, 5, 7, 10, 12, 15, 18, 20]
for i, v in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(f"{v}\n")
