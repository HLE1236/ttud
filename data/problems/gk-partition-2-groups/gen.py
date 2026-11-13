import sys, os, random
tdir = sys.argv[1]
rng = random.Random(202313)
cases = [
    "5\n208 750 114 184 538\n",
    "1\n42\n",
    "2\n100 100\n",
    "4\n1 1 1 100\n",
]
for sz, mx in [(8, 50), (12, 500), (16, 2000), (18, 5000), (20, 10000), (20, 9999)]:
    arr = [str(rng.randint(1, mx)) for _ in range(sz)]
    cases.append(f"{sz}\n" + " ".join(arr) + "\n")
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
