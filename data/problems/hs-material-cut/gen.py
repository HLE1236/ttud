import sys, os, random
tdir = sys.argv[1]; rng = random.Random(30401)
cases = ["7 5\n4\n1 5\n6 2\n2 2\n4 3\n", "4 4\n2\n3 3\n2 2\n", "10 10\n3\n10 5\n5 5\n5 5\n"]
for H, W, n in [(6, 6, 4), (7, 8, 5), (8, 8, 6), (9, 9, 7), (10, 8, 6), (10, 10, 8), (10, 10, 9)]:
    lines = [f"{H} {W}", str(n)]
    for _ in range(n):
        lines.append(f"{rng.randint(1, max(2, H//2))} {rng.randint(1, max(2, W//2))}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
