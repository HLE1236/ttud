import sys, os, random
random.seed(508)
tdir = sys.argv[1]
cases = [
    "5 5\n2 5 3 3 2\n3 2 -1 3 4\n2 1 2 3 3\n4 -2 5 2 4\n3 6 2 2 1\n",
    "3 3\n1 0 1\n-1 0 -2\n1 0 1\n",
]
for idx in range(8):
    N = 6 if idx < 3 else (12 if idx < 6 else 20)
    M = 6 if idx < 3 else (12 if idx < 6 else 20)
    g = [[random.randint(0, 50) for _ in range(M)] for _ in range(N)]
    sr, sc = random.randrange(N), random.randrange(M)
    g[sr][sc] = -1
    exits = random.randint(1, 4)
    for _ in range(exits * 5):
        er, ec = random.randrange(N), random.randrange(M)
        if abs(er - sr) + abs(ec - sc) > 1 and g[er][ec] >= 0:
            g[er][ec] = -2
            exits -= 1
            if exits == 0: break
    lines = [f"{N} {M}"] + [" ".join(map(str, r)) for r in g]
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
