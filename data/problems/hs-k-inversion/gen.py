import sys, os, random
random.seed(407)
tdir = sys.argv[1]
cases = [
    "6 3\n3 5 4 1 2 6\n",
    "5 2\n5 4 3 2 1\n",
    "5 4\n1 2 3 4 5\n",
]
for idx in range(7):
    n = 20 if idx < 3 else (2000 if idx < 5 else 100000)
    k = random.randint(2, 10)
    p = list(range(1, n + 1))
    if idx == 6:
        p.reverse()
    else:
        random.shuffle(p)
    cases.append(f"{n} {k}\n" + " ".join(map(str, p)) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
