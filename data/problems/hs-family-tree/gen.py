import sys, os, random
random.seed(502)
tdir = sys.argv[1]
cases = [
    "Peter Newman\nMichael Thomas\nJohn David\nPaul Mark\nStephan Mark\nPierre Thomas\nMark Newman\nBill David\nDavid Newman\nThomas Mark\n***\ndescendants Newman\ndescendants Mark\ndescendants David\ngeneration Mark\n***\n",
    "Child Root\n***\ndescendants Root\ndescendants Child\ngeneration Root\ngeneration Child\n***\n",
]
for idx in range(8):
    n = 20 if idx < 3 else (500 if idx < 6 else 5000)
    names = [f"P{i}" for i in range(n)]
    lines = []
    for i in range(1, n):
        p = random.randrange(i)
        lines.append(f"{names[i]} {names[p]}")
    lines.append("***")
    q = min(n * 2, 3000)
    for _ in range(q):
        cmd = random.choice(["descendants", "generation"])
        lines.append(f"{cmd} {random.choice(names)}")
    lines.append("***")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
