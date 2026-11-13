import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202421)
cases = [
    "2 2\n9 13 3500\n17 20 4500\n0 0 0 0 0 0 0 0 0 2 7 2 1 0 0 3 2 1 1 1 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 1 2 0 0 0 0 2 1 3 2 0 0 0 0\n",
    "1 1\n0 24 1000\n" + " ".join(["1"]*24) + "\n",
    "1 2\n5 8 2000\n" + " ".join(["5"]*24) + "\n" + " ".join(["0"]*24) + "\n",
]
for K, N in [(2, 10), (3, 50), (4, 100), (5, 200), (6, 300), (8, 400), (10, 500)]:
    pts = sorted(rng.sample(range(0, 25), 2 * K))
    lines = [f"{K} {N}"]
    for i in range(K):
        lines.append(f"{pts[2*i]} {pts[2*i+1]} {rng.randint(100, 10000)}")
    for _ in range(N):
        lines.append(" ".join(str(rng.randint(0, 50)) for _ in range(24)))
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
