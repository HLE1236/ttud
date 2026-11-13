import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202514)
cases = [
    "3 55\n10 20 30\n3 7 9\n",
    "1 10\n10\n100\n",
    "1 9\n10\n100\n",
    "4 15\n2 3 5 7\n3 5 9 12\n",
]
for n, W in [(10, 100), (20, 500), (50, 2000), (80, 5000), (100, 10000), (100, 10000)]:
    c = [str(rng.randint(1, max(2, W // 3))) for _ in range(n)]
    v = [str(rng.randint(1, 10000)) for _ in range(n)]
    cases.append(f"{n} {W}\n" + " ".join(c) + "\n" + " ".join(v) + "\n")
for idx, content in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(content)
