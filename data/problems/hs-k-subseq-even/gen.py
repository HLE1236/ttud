import sys, os, random
random.seed(201)
tdir = sys.argv[1]
cases = [
    "6 3\n2 4 5 1 1 2\n",
    "2 1\n2 4\n",
    "5 2\n1 3 5 7 9\n",
]
for idx in range(7):
    n = 50 if idx < 3 else (5000 if idx < 5 else 100000)
    k = random.randint(1, max(1, n // 2))
    arr = [str(random.randint(1, 10000)) for _ in range(n)]
    cases.append(f"{n} {k}\n" + " ".join(arr) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
