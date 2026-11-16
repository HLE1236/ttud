import sys, os
tdir = sys.argv[1]
cases = [1, 0, 2, 3, 5, 10, 50, 200, 500, 1000]
for i, v in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(f"{v}\n")
