import sys, os, random
random.seed(203)
tdir = sys.argv[1]
cases = [
    "insert 20\ninsert 10\ninsert 26\ninsert 7\ninsert 15\ninsert 23\ninsert 30\ninsert 3\ninsert 8\n#\n",
    "insert 5\ninsert 5\ninsert 5\n#\n",
    "\n".join(f"insert {i}" for i in range(1, 20)) + "\n#\n",
]
for idx in range(7):
    n = 30 if idx < 4 else 1000
    lines = [f"insert {random.randint(1, 500)}" for _ in range(n)] + ["#"]
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
