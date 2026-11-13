import os, sys, random
tdir = sys.argv[1]
random.seed(20250208)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "4 5\n3 4 1 5 7\n4 3 7 1 6\n5 2 6 2 8\n3 4 2 1 6")
w(2, "1 5\n-1 -2 10 -3 -4")
w(3, "5 1\n-5\n-2\n-9\n-1\n-4")
for idx in range(4, 11):
    m = random.choice([10, 50, 200, 600, 1000])
    n = random.choice([10, 50, 200, 600, 1000])
    lines = [f"{m} {n}"]
    for _ in range(m):
        lines.append(" ".join(str(random.randint(-1000, 1000)) for _ in range(n)))
    w(idx, "\n".join(lines))
