import os, sys, random
tdir = sys.argv[1]
random.seed(20250309)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "9\n9 5 10 4 3 2 6 1 8\n1 5\n2 3\n2 4\n2 6\n3 5\n6 7\n6 8\n6 9")
w(2, "1\n100")
for idx in range(3, 11):
    n = random.choice([20, 300, 3000, 30000, 80000])
    ws = [random.randint(1, 1000) for _ in range(n)]
    lines = [str(n), " ".join(map(str, ws))]
    for v in range(2, n + 1):
        lines.append(f"{random.randint(1, v - 1)} {v}")
    w(idx, "\n".join(lines))
