import sys, os
tdir = sys.argv[1]
cases = [
    "6 3 12\n",
    "3 3 5\n",
    "6 1 7\n",
    "6 5 4\n",
    "1 100 100\n",
    "10 10 55\n",
    "50 30 700\n",
    "100 50 2500\n",
    "100 100 5000\n",
    "100 100 10000\n",
]
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
