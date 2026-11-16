import sys, os, random
random.seed(101)
tdir = sys.argv[1]
cases = [
    "AI\nArtificial Intelligence\nRecently, AI is a key technology. AI enable efficient operations in many fields.\n",
    "a\naa\naaaa\n",
    "xyz\nabc\nhello world without match\n",
    "foo bar\nbaz\nfoo bar is foo bar and foo  bar\n",
    "ab\nba\nababab\n",
    "test\n\ntest one test two test\n",
]
words = ["algo", "data", "tree", "graph", "dp", "hust", "code", "cpp", "stack", "queue"]
for _ in range(4):
    p1 = random.choice(words)
    p2 = random.choice(words) + "_" + random.choice(words)
    t = " ".join(random.choice(words) for _ in range(80))
    cases.append(f"{p1}\n{p2}\n{t}\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w", encoding="utf-8").write(c)
