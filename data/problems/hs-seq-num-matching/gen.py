import sys, os, random
random.seed(204)
tdir = sys.argv[1]
cases = [
    "10 3\n1 2 3 4 5 1 2 3 7 8\n1 2 3\n",
    "6 2\n5 5 5 5 5 5\n5 5\n",
    "5 6\n1 2 3 4 5\n1 2 3 4 5 6\n",
    "100000 5000\n" + " ".join(["1"] * 100000) + "\n" + " ".join(["1"] * 4999 + ["2"]) + "\n",
]
for idx in range(6):
    n = 500 if idx < 3 else 50000
    m = random.randint(2, 50)
    b = [random.randint(1, 3) for _ in range(m)]
    a = [random.randint(1, 3) for _ in range(n)]
    for _ in range(5):
        pos = random.randint(0, max(0, n - m))
        a[pos:pos+m] = b
    cases.append(f"{n} {m}\n" + " ".join(map(str, a)) + "\n" + " ".join(map(str, b)) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
