import sys, os, random, string
random.seed(108)
tdir = sys.argv[1]
cases = [
    "computer\nuniversity\nschool\ntechnology\nphone\n*\nfind school\nfind book\ninsert book\nfind algorithm\nfind book\ninsert book\n***\n",
    "*\nfind a\ninsert a\nfind a\n***\n",
]
def rstr(L=10):
    return "".join(random.choice(string.ascii_lowercase) for _ in range(L))
for idx in range(8):
    n = 50 if idx < 3 else (5000 if idx < 6 else 50000)
    q = 50 if idx < 3 else (5000 if idx < 6 else 50000)
    pool = [rstr(random.randint(5, 18)) for _ in range(n)]
    lines = pool + ["*"]
    for _ in range(q):
        cmd = random.choice(["find", "insert"])
        k = random.choice(pool) if random.random() < 0.6 else rstr(random.randint(5, 18))
        lines.append(f"{cmd} {k}")
    lines.append("***")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
