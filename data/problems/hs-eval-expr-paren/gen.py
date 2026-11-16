import sys, os, random
random.seed(104)
tdir = sys.argv[1]
def gen_expr(depth=0):
    if depth >= 3 or (depth > 0 and random.random() < 0.55):
        return str(random.randint(1, 500))
    k = random.randint(2, 4)
    parts = []
    for i in range(k):
        if i > 0:
            parts.append(random.choice(["+", "*", " + ", " * "]))
        sub = gen_expr(depth + 1)
        if random.random() < 0.45:
            sub = "(" + sub + ")"
        parts.append(sub)
    return "".join(parts)

cases = [
    "(2+4*10)*(5) + 7\n",
    "((2+4*10)*(5) + 7\n",
    "()\n",
    "(2)(3)\n",
    "((1+2)*(3+4))*(5+6*7)\n",
    "1 + 2 * (3 + 4 * (5 + 6)))\n",
    "5+*3\n",
    gen_expr(0) + "\n",
    "+".join(gen_expr(0) for _ in range(50)) + "\n",
    "*".join(f"({gen_expr(0)})" for _ in range(80)) + "\n",
]
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
