import sys, os
tdir = sys.argv[1]
cases = ["3\n10\n", "4\n111\n", "5\n0\n", "6\n01\n", "10\n101\n", "12\n0010\n", "15\n11011\n", "18\n01010\n", "20\n11111\n", "20\n1010\n"]
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
