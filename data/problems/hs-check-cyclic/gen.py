import os, sys, random
tdir = sys.argv[1]
random.seed(20250305)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "6 8\n1 2\n1 3\n1 4\n2 3\n3 5\n4 5\n4 6\n5 6")
w(2, "5 4\n1 2\n2 3\n3 4\n4 5")
for idx in range(3, 11):
    n = random.choice([30, 500, 5000, 50000])
    is_tree = (idx % 2 == 0)
    edges = [(random.randint(1, i - 1), i) for i in range(2, n + 1)]
    if not is_tree:
        edges.append((1, n))
        m_extra = random.randint(0, min(10000, n // 2))
        for _ in range(m_extra):
            edges.append((random.randint(1, n), random.randint(1, n)))
    else:
        edges = edges[:max(0, n - random.randint(1, min(n, 5)))]
    w(idx, f"{n} {len(edges)}\n" + "\n".join(f"{u} {v}" for u, v in edges))
