import sys, os, random
random.seed(102)
tdir = sys.argv[1]
def gen_valid(n):
    pairs = [('(', ')'), ('[', ']'), ('{', '}')]
    res = []
    st = []
    for i in range(n):
        rem = n - i
        if not st or (rem > len(st) and random.random() < 0.55):
            op, cl = random.choice(pairs)
            res.append(op)
            st.append(cl)
        else:
            res.append(st.pop())
    return "".join(res)

cases = [
    "(()[][]{}){}{}[][]({[]()})\n",
    "([]{()]()[])\n",
    "(\n",
    ")\n",
    "(((())))\n",
    gen_valid(100) + "\n",
    gen_valid(999) + "(\n",
    gen_valid(50000) + "\n",
]
s8 = list(gen_valid(50000))
s8[25000] = '}' if s8[25000] != '}' else ']'
cases.append("".join(s8) + "\n")
cases.append(gen_valid(500000) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
