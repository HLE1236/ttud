import sys, os, random
random.seed(406)
tdir = sys.argv[1]
cases = [
    "6 2 3\n3 5 9 6 7 4\n",
    "6 2 3\n2 3 1 5 9 2\n",
    "5 5 5\n10 20 30 40 50\n",
]
for idx in range(7):
    n = 50 if idx < 3 else (5000 if idx < 5 else 100000)
    L1 = random.randint(1, max(1, n // 10))
    L2 = random.randint(L1, min(n, L1 + max(5, n // 5)))
    a = [str(random.randint(0, 1000)) for _ in range(n)]
    cases.append(f"{n} {L1} {L2}\n" + " ".join(a) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
