import sys, os, random
random.seed(105)
tdir = sys.argv[1]
cases = [
    "5\n5 4 3 2 1\naddlast 3\naddlast 10\naddfirst 1\naddafter 10 4\nremove 1\n#\n",
    "3\n1 2 3\naddbefore 9 1\naddafter 8 3\nreverse\nremove 2\n#\n",
    "1\n42\nremove 42\naddfirst 7\naddlast 9\nreverse\n#\n",
]
for idx in range(7):
    n = random.randint(10, 300 if idx < 5 else 1000)
    vals = random.sample(range(1, 5000), n)
    lines = [str(n), " ".join(map(str, vals))]
    q = random.randint(20, 500)
    for _ in range(q):
        op = random.choice(["addlast", "addfirst", "addafter", "addbefore", "remove", "reverse"])
        if op in ("addlast", "addfirst", "remove"):
            lines.append(f"{op} {random.randint(1, 1500)}")
        elif op in ("addafter", "addbefore"):
            lines.append(f"{op} {random.randint(1, 1500)} {random.choice(vals)}")
        else:
            lines.append("reverse")
    lines.append("#")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
