import sys, os, random
random.seed(505)
tdir = sys.argv[1]
sample = "4\n3 3\n1 2\n1 3\n2 3\n4 4\n1 2\n2 3\n3 4\n1 4\n4 5\n1 2\n1 3\n2 3\n2 4\n3 4\n4 5\n1 2\n1 3\n1 4\n2 4\n3 4\n#\n3\n3 2\n1 2\n1 3\n4 5\n1 3\n1 4\n2 3\n2 4\n3 4\n4 4\n1 2\n1 3\n2 4\n3 4\n"
cases = [sample]
def rand_graph(n):
    all_e = [(i, j) for i in range(1, n + 1) for j in range(i + 1, n + 1)]
    m = random.randint(0, len(all_e))
    return n, random.sample(all_e, m)
def perm_graph(n, edges):
    p = list(range(1, n + 1))
    random.shuffle(p)
    return n, [(p[u - 1], p[v - 1]) for u, v in edges]

for idx in range(9):
    N = 20 if idx < 4 else 300
    K = 20 if idx < 4 else 300
    max_n = 5 if idx < 4 else 7
    gs = [rand_graph(random.randint(3, max_n)) for _ in range(N)]
    lines = [str(N)]
    for n, es in gs:
        lines.append(f"{n} {len(es)}")
        for u, v in es: lines.append(f"{u} {v}")
    lines.append("#")
    lines.append(str(K))
    for _ in range(K):
        if random.random() < 0.55:
            n, es = perm_graph(*random.choice(gs))
        else:
            n, es = rand_graph(random.randint(3, max_n))
        lines.append(f"{n} {len(es)}")
        for u, v in es: lines.append(f"{u} {v}")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
