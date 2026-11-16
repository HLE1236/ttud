import sys, os, random
random.seed(202)
tdir = sys.argv[1]
cases = [
    "10\n2 8 5 1 10 5 9 9 3 5\n5\nupdate 6 2\nupdate 2 2\nupdate 3 8\nsum 2\nsum 3\n",
    "1\n5\n3\nsum 1\nupdate 1 10\nsum 1\n",
]
for idx in range(8):
    n = 100 if idx < 3 else (10000 if idx < 6 else 200000)
    q = 100 if idx < 3 else (10000 if idx < 6 else 200000)
    lines = [str(n), " ".join(str(random.randint(1, 100)) for _ in range(n)), str(q)]
    for _ in range(q):
        if random.random() < 0.5:
            lines.append(f"sum {random.randint(1, n)}")
        else:
            lines.append(f"update {random.randint(1, n)} {random.randint(1, 1000)}")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
