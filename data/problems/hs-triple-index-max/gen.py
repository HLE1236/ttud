import sys, os, random
random.seed(205)
tdir = sys.argv[1]
cases = [
    "10\n42 68 35 1 70 25 79 59 63 65\n",
    "3\n1 1000 1\n",
    "3\n1000 1 1000\n",
]
for idx in range(7):
    n = 20 if idx < 3 else (5000 if idx < 5 else 300000)
    arr = [str(random.randint(1, 1000)) for _ in range(n)]
    cases.append(f"{n}\n" + " ".join(arr) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
