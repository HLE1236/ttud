import os, sys, random
tdir = sys.argv[1]
random.seed(20250322)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "2\n5 5\n1 2\n1 3\n2 4\n2 5\n3 5\n7 13\n1 3\n1 5\n1 7\n2 4\n2 5\n2 6\n3 4\n3 5\n3 7\n4 6\n4 7\n5 7\n6 7")
for idx in range(2, 11):
    T = random.randint(2, 4)
    lines = [str(T)]
    for t in range(T):
        n = random.randint(6, 15)
        if t % 2 == 0:
            perm = list(range(1, n + 1)); random.shuffle(perm)
            edges = set((min(perm[i], perm[(i+1)%n]), max(perm[i], perm[(i+1)%n])) for i in range(n))
            for _ in range(n):
                u = random.randint(1, n); v = random.randint(1, n)
                if u != v: edges.add((min(u, v), max(u, v)))
        else:
            # No Hamilton cycle (star/pendant/sparse)
            edges = set((1, i) for i in range(2, n + 1))
            edges.add((2, 3))
        lines.append(f"{n} {len(edges)}")
        lines.extend(f"{u} {v}" for u, v in edges)
    w(idx, "\n".join(lines))
