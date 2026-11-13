import sys, os, random
tdir = sys.argv[1]; rng = random.Random(20501)
cases = ["6 2\n4 4 4 4 2 4\n0 0 0 0 0 0\n0 0 0 0 0 0\n0 0 0 0 0 0\n0 0 1 0 0 0\n0 0 1 0 0 0\n1 0 0 0 0 0\n"]
for N, M, p in [(4, 2, 0.2), (6, 3, 0.2), (8, 3, 0.25), (10, 4, 0.2), (12, 4, 0.2), (14, 4, 0.25), (15, 5, 0.2), (16, 5, 0.15), (16, 4, 0.3)]:
    perm = list(range(N)); rng.shuffle(perm)
    A = [[0]*N for _ in range(N)]
    for i in range(N):
        for j in range(i + 1, N):
            if rng.random() < p:
                A[perm[i]][perm[j]] = 1
    lines = [f"{N} {M}", " ".join(str(rng.randint(1, 5)) for _ in range(N))]
    for r in A: lines.append(" ".join(map(str, r)))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
