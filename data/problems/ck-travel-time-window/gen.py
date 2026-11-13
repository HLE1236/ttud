import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202415)
cases = [
    "4 5\n0 1 10 1 1 5 0 100\n0 2 20 2 1 8 5 50\n1 2 15 1 1 10 15 100\n2 3 15 1 1 7 20 100\n1 3 10 1 1 6 25 100\n0 3\n",
    "3 2\n0 1 5 2 1 10 0 8\n1 2 5 2 1 10 0 20\n0 2\n",
    "3 3\n0 1 10 1 1 1 0 100\n0 1 2 1 1 100 0 100\n1 2 5 1 1 1 0 8\n0 2\n",
]
for N, M in [(8, 20), (15, 50), (30, 150), (50, 400), (70, 1000), (90, 2000), (100, 4000)]:
    lines = [f"{N} {M}"]
    for _ in range(M):
        u = rng.randrange(N); v = rng.randrange(N)
        d = rng.randint(1, 50); t = rng.randint(1, 5); tp = rng.choice([1, 2])
        toll = rng.randint(0, 500)
        op = rng.randint(0, 200)
        cl = op + d * t + rng.randint(20, 1000)
        lines.append(f"{u} {v} {d} {t} {tp} {toll} {op} {cl}")
    lines.append(f"0 {N - 1}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
