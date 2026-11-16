import sys, os
tdir = sys.argv[1]
cases = ["3 5\n", "1 1\n", "1 999\n", "999 999\n", "500 999\n", "250 500\n", "10 20\n", "100 200\n", "400 800\n", "998 999\n"]
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
