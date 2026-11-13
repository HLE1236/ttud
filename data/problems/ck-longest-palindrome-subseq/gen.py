import sys, os, random, string
tdir = sys.argv[1]
rng = random.Random(2025104)
cases = [
    "bbabcbcab\n",
    "a\n",
    "aaaaaaa\n",
    "abcdefg\n",
    "character\n",
]
for sz, alpha in [(20, "abc"), (50, "abcd"), (200, "abcdefgh"), (500, string.ascii_lowercase), (800, "abcdef"), (1000, string.ascii_lowercase)]:
    s = "".join(rng.choice(alpha) for _ in range(sz))
    cases.append(s + "\n")
for idx, c in enumerate(cases[:10], 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
