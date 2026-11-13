import os, sys, random
tdir = sys.argv[1]
random.seed(20250304)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "6 6\n1 2\n1 3\n2 5\n2 6\n4 5\n4 6")
w(2, "3 3\n1 2\n2 3\n3 1")
for idx in range(3, 11):
    n = random.choice([30, 500, 5000, 30000])
    is_bip = (idx % 2 == 1)
    sides = [random.randint(0, 1) for _ in range(n + 1)]
    edges = []
    m = min(80000, n * 2)
    for _ in range(m):
        u = random.randint(1, n); v = random.randint(1, n)
        if is_bip:
            while sides[u] == sides[v]:
                v = random.randint(1, n)
        edges.append(f"{u} {v}")
    if not is_bip and n >= 3:
        edges.extend(["1 2", "2 3", "3 1"])
    w(idx, f"{n} {len(edges)}\n" + "\n".join(edges))
