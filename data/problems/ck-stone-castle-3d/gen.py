import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2023152)
cases = [
    "2\n5 10 3\n3\n3 9 3\n2 4 3\n2 2 2\n5 5 2\n2\n3 2 3\n1 3 2\n",
    "1\n1 10 10\n2\n1 4 4\n1 3 7\n",
    "1\n4 4 4\n1\n4 4 4\n",
]
for T, maxD, N in [(2, 10, 5), (2, 20, 8), (2, 35, 15), (2, 50, 20), (2, 70, 30), (3, 90, 40), (2, 100, 50)]:
    lines = [str(T)]
    for _ in range(T):
        L = rng.randint(max(3, maxD // 2), maxD)
        W = rng.randint(max(3, maxD // 2), maxD)
        H = rng.randint(max(3, maxD // 2), maxD)
        lines.append(f"{L} {W} {H}")
        lines.append(str(N))
        for _ in range(N):
            lines.append(f"{rng.randint(1, L)} {rng.randint(1, W)} {rng.randint(1, H)}")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
