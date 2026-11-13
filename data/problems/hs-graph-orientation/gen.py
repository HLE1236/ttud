import os, sys, random
tdir = sys.argv[1]
random.seed(20250321)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "6 8\n1 2\n1 3\n1 4\n2 4\n3 4\n4 5\n4 6\n5 6")
w(2, "9 12\n1 2\n1 3\n1 4\n2 4\n3 4\n4 5\n5 6\n5 7\n5 8\n6 8\n7 8\n8 9")
for idx in range(3, 11):
    n = random.choice([10, 50, 300, 1500, 4000])
    # Cycle + extra chords => guaranteed bridgeless connected! Or add pendant vertex for -1 cases
    edges = [(i, (i % n) + 1) for i in range(1, n + 1)]
    extra = n
    seen = set((min(u, v), max(u, v)) for u, v in edges)
    for _ in range(extra):
        u = random.randint(1, n); v = random.randint(1, n)
        if u != v and (min(u, v), max(u, v)) not in seen:
            seen.add((min(u, v), max(u, v)))
            edges.append((u, v))
    if idx % 3 == 0:
        # Attach a bridge to a new node n+1
        edges.append((1, n + 1))
        n += 1
    w(idx, f"{n} {len(edges)}\n" + "\n".join(f"{u} {v}" for u, v in edges))
