import os, sys, random
tdir = sys.argv[1]
random.seed(20250318)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "3 4\n2 1 4\n2 1 3\n1 2")
for idx in range(2, 11):
    n = random.choice([20, 200, 1000, 5000, 10000])
    m = random.choice([20, 200, 1000, 5000, 10000])
    lines = [f"{n} {m}"]
    for _ in range(n):
        k = random.randint(1, min(m, 10))
        cand = random.sample(range(1, m + 1), k)
        lines.append(f"{k} " + " ".join(map(str, cand)))
    w(idx, "\n".join(lines))
