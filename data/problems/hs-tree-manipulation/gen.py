import sys, os, random
random.seed(503)
tdir = sys.argv[1]
cases = [
    "MakeRoot 10\nInsert 11 10\nInsert 1 10\nInsert 3 10\nInOrder\nInsert 5 11\nInsert 4 11\nInsert 8 3\nPreOrder\nInsert 2 3\nInsert 7 3\nInsert 6 4\nInsert 9 4\nInOrder\nPostOrder\n*\n",
    "MakeRoot 1\nPreOrder\nInOrder\nPostOrder\n*\n",
]
for idx in range(8):
    n = 15 if idx < 4 else 200
    nodes = [1]
    lines = ["MakeRoot 1"]
    for u in range(2, n + 1):
        v = random.choice(nodes)
        lines.append(f"Insert {u} {v}")
        nodes.append(u)
        if random.random() < 0.15:
            lines.append(random.choice(["PreOrder", "InOrder", "PostOrder"]))
    lines.extend(["PreOrder", "InOrder", "PostOrder", "*"])
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
