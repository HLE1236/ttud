import os, sys, random
tdir = sys.argv[1]
random.seed(20250216)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "4\n0 1 1 9\n1 0 9 3\n1 9 0 2\n9 3 2 0")
for idx in range(2, 11):
    n = random.choice([10, 30, 80, 150, 300, 600])
    mat = [[0] * n for _ in range(n)]
    pts = [(random.randint(0, 1000), random.randint(0, 1000)) for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            d = int(((pts[i][0]-pts[j][0])**2 + (pts[i][1]-pts[j][1])**2) ** 0.5) + 1
            mat[i][j] = mat[j][i] = d
    lines = [str(n)] + [" ".join(map(str, row)) for row in mat]
    w(idx, "\n".join(lines))
