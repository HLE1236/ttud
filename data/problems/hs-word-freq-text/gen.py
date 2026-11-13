import sys, os, random
tdir = sys.argv[1]; rng = random.Random(10106)
cases = [
    "abc  def abc\nabc abcd def\n",
    "Hello, world! Hello 123.\n123; hello\n",
]
vocab = ["ttud", "hust", "algo", "dp", "graph", "tree", "2025", "cpp", "array", "map", "set"]
delims = [" ", "  ", "\n", ", ", ". ", "; ", "\t", "! "]
for words in [20, 50, 200, 1000, 3000, 5000, 10000, 20000]:
    parts = []
    for _ in range(words):
        parts.append(rng.choice(vocab) + rng.choice(delims))
    cases.append("".join(parts) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
