import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202424)
cases = [
    "4 5\n0 1 3 2 2\n1 2 2 3 2\n2 3 4 1 1\n0 2 5 5 1\n3 0 1 4 1\n0 3\n",
    "3 1\n0 1 5 2 1\n0 2\n",
    "2 1\n1 0 10 3 1\n0 1\n",
]
for N, M in [(10, 20), (20, 80), (40, 200), (60, 500), (80, 1500), (100, 5000), (100, 10000)]:
    lines = [f"{N} {M}"]
    for _ in range(M):
        u = rng.randrange(N); v = rng.randrange(N)
        d = rng.randint(1, 500); t = rng.randint(1, 5); tp = rng.choice([1, 2])
        lines.append(f"{u} {v} {d} {t} {tp}")
    lines.append(f"0 {N - 1}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
