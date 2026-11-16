import sys, os, random, string
random.seed(106)
tdir = sys.argv[1]
cases = [
    "4 1000\na\nab\nabc\nabcd\n",
    "3 1\nabc\nxyz\nz\n",
    "3 99999\nz\nzz\n" + "a" * 200 + "\n",
]
for idx in range(7):
    n = 100 if idx < 4 else (5000 if idx < 6 else 50000)
    m = random.randint(2, 100000)
    lines = [f"{n} {m}"]
    for _ in range(n):
        L = random.randint(1, 20 if idx < 6 else 60)
        lines.append("".join(random.choice(string.ascii_lowercase) for _ in range(L)))
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
