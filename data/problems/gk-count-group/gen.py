import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202512)
cases = [
    "5\n1 2 4 3 3\n3\n",
    "1\n4\n4\n",
    "1\n1\n2\n",
    "6\n2 2 2 2 2 2\n2\n",
    "6\n1 1 1 2 3 4\n4\n",
]
for sz in [100, 1000, 10000, 50000, 100000]:
    arr = [str(rng.randint(1, 4)) for _ in range(sz)]
    t = rng.randint(1, 4)
    cases.append(f"{sz}\n" + " ".join(arr) + f"\n{t}\n")
for i, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{i:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
