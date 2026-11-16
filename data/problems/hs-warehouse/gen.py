import sys, os, random
random.seed(404)
tdir = sys.argv[1]
cases = [
    "6 6 2\n6 8 5 10 11 6\n1 2 2 3 3 2\n",
    "3 1 1\n5 10 3\n2 2 2\n",
    "1 5 1\n9\n5\n",
]
for idx in range(7):
    N = 30 if idx < 3 else (300 if idx < 5 else 1000)
    T = random.randint(10, 100)
    D = random.randint(1, 10)
    a = [str(random.randint(1, 10)) for _ in range(N)]
    t = [str(random.randint(1, 10)) for _ in range(N)]
    cases.append(f"{N} {T} {D}\n" + " ".join(a) + "\n" + " ".join(t) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
