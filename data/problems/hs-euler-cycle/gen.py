import os, sys, random
tdir = sys.argv[1]
random.seed(20250320)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "7 12\n1 2\n1 3\n2 3\n2 4\n2 5\n3 4\n3 6\n4 5\n4 6\n5 6\n5 7\n6 7")
w(2, "4 3\n1 2\n2 3\n3 4")
for idx in range(3, 11):
    n = random.choice([8, 20, 100, 300, 800])
    if idx % 3 == 0:
        # Non-Eulerian (disconnected or odd degree)
        edges = [(1, 2), (2, 3), (3, 1), (4, 5), (5, 6), (6, 4)]
        w(idx, f"{max(n,6)} {len(edges)}\n" + "\n".join(f"{u} {v}" for u, v in edges))
    else:
        # Build Eulerian tour directly by walking a closed circuit covering 1..n
        walk = [1] + [random.randint(1, n) for _ in range(n * 2)]
        # Make sure connected through 1..n
        nodes = list(range(2, n + 1)); random.shuffle(nodes)
        walk = [1] + nodes + [random.randint(1, n) for _ in range(n)] + [1]
        edges = [(walk[i], walk[i+1]) for i in range(len(walk) - 1) if walk[i] != walk[i+1]]
        # Ensure still Eulerian by adding closing edge from walk ends if self-loops skipped -> even simpler: swap neighbor if equal!
        walk = [1] + nodes
        for _ in range(n):
            nxt = random.randint(1, n)
            if nxt != walk[-1]: walk.append(nxt)
        if walk[-1] != 1: walk.append(1)
        edges = [(walk[i], walk[i+1]) for i in range(len(walk) - 1)]
        random.shuffle(edges)
        w(idx, f"{n} {len(edges)}\n" + "\n".join(f"{u} {v}" for u, v in edges))
