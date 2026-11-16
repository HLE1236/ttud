import sys, os, random
random.seed(509)
tdir = sys.argv[1]
cases = [
    "6 6 2\n1 3 1\n1 2 1\n2 5 1\n3 4 2\n4 5 3\n6 2 1\n1 5\n6 5\n",
    "3 2 2\n1 2 1\n2 3 1\n1 3\n1 3\n",
]
for idx in range(8):
    N = 12 if idx < 4 else (40 if idx < 7 else 80)
    edges = {}
    # strongly connected cycle + random edges
    for i in range(1, N + 1):
        j = (i % N) + 1
        edges[(i, j)] = random.randint(1, 5)
    for _ in range(N * 3):
        u, v = random.randint(1, N), random.randint(1, N)
        if u != v and (u, v) not in edges:
            edges[(u, v)] = random.randint(1, 4)
    K = 15 if idx < 4 else (80 if idx < 7 else 250)
    lines = [f"{N} {len(edges)} {K}"]
    for (u, v), c in edges.items():
        lines.append(f"{u} {v} {c}")
    for _ in range(K):
        s = random.randint(1, N)
        d = random.randint(1, N)
        while d == s:
            d = random.randint(1, N)
        lines.append(f"{s} {d}")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
