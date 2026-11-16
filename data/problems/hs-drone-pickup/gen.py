import sys, os, random
random.seed(405)
tdir = sys.argv[1]
cases = [
    "6 3\n3 1 4 2 2 2\n2 3 1 1 3 1\n",
    "5 1\n10 20 30 40 50\n2 5 5 5 5\n",
    "1 3\n7\n1\n",
]
for idx in range(7):
    N = 50 if idx < 3 else (500 if idx < 5 else 3000)
    K = random.randint(5, 50)
    c = [str(random.randint(1, 1000)) for _ in range(N)]
    a = [str(random.randint(1, 50)) for _ in range(N)]
    cases.append(f"{N} {K}\n" + " ".join(c) + "\n" + " ".join(a) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
