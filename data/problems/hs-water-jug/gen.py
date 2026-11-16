import sys, os, random
random.seed(501)
tdir = sys.argv[1]
cases = [
    "6 8 4\n",
    "6 8 5\n",
    "1 1 1\n",
    "10 20 30\n",
    "899 900 1\n",
    "700 850 150\n",
    "123 456 78\n",
    "500 750 100\n",
    "887 889 450\n",
    "900 897 300\n",
]
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
