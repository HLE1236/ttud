import sys, os, random
random.seed(107)
tdir = sys.argv[1]
cases = [
    "5\n1 4 3 1 4\n",
    "1\n1000000000\n",
    "6\n7 7 7 7 7 7\n",
]
for idx in range(7):
    n = 100 if idx < 3 else (10000 if idx < 5 else 100000)
    hi = 50 if idx % 2 == 0 else 10**9
    arr = [str(random.randint(1, hi)) for _ in range(n)]
    cases.append(f"{n}\n" + " ".join(arr) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
