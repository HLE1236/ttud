import sys, os, random
random.seed(507)
tdir = sys.argv[1]
cases = [
    "4\n7 8 13 10\n7 5 6 9\n6 19 12 11\n24 23 18 10\n",
    "1\n42\n",
]
for idx in range(8):
    n = 15 if idx < 3 else (80 if idx < 6 else 200)
    lines = [str(n)]
    for _ in range(n):
        lines.append(" ".join(str(random.randint(1, 10000)) for _ in range(n)))
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
