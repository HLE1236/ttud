import sys, os, random
random.seed(207)
tdir = sys.argv[1]
sample = "0 0 3 4 0 0 0 8 9\n0 0 6 7 8 9 0 2 3\n0 8 0 0 2 3 4 5 6\n0 0 4 0 6 5 0 9 7\n0 6 0 0 9 0 0 1 4\n0 0 7 2 0 4 3 6 5\n0 3 0 6 0 2 0 7 8\n0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0\n"
base = [
    [5,3,4,6,7,8,9,1,2],
    [6,7,2,1,9,5,3,4,8],
    [1,9,8,3,4,2,5,6,7],
    [8,5,9,7,6,1,4,2,3],
    [4,2,6,8,5,3,7,9,1],
    [7,1,3,9,2,4,8,5,6],
    [9,6,1,5,3,7,2,8,4],
    [2,8,7,4,1,9,6,3,5],
    [3,4,5,2,8,6,1,7,9],
]
cases = [sample]
for idx in range(9):
    g = [row[:] for row in base]
    # permute digits
    perm = list(range(1, 10))
    random.shuffle(perm)
    for r in range(9):
        for c in range(9):
            g[r][c] = perm[g[r][c] - 1]
    zeros = 10 + idx * 4  # up to 42 zeros
    coords = [(r, c) for r in range(9) for c in range(9)]
    for r, c in random.sample(coords, zeros):
        g[r][c] = 0
    if idx == 8:
        # unsolvable board
        g[0][0] = g[0][1] if g[0][1] != 0 else 1
        g[0][1] = g[0][0]
    cases.append("\n".join(" ".join(map(str, row)) for row in g) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
