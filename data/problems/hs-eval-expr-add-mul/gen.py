import sys, os, random
random.seed(103)
tdir = sys.argv[1]
cases = [
    "5+7*3*10*10\n",
    "5*+ 7*3*10*10\n",
    "42\n",
    "+5*3\n",
    "5+3*\n",
    "5 7 + 3\n",
    "10 + 20 * 30 + 40 * 50\n",
    "1000000000 * 1000000000 + 7\n",
]
for k in [200, 9999]:
    parts = [str(random.randint(1, 1000))]
    for _ in range(k):
        parts.append(random.choice(["+", "*", " + ", " * "]))
        parts.append(str(random.randint(1, 100000)))
    cases.append("".join(parts) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
