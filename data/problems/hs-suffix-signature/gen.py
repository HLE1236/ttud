import sys, os, random
random.seed(110)
tdir = sys.argv[1]
cases = [
    "2\nababaa\naa\n",
    "1\na\n",
    "2\n" + "a" * 100000 + "\n" + "ab" * 50000 + "\n",
    "3\nabcdefg\nabaabaaba\nzzzyzzzy\n",
]
for idx in range(6):
    t = random.randint(2, 6)
    lines = [str(t)]
    for _ in range(t):
        n = 500 if idx < 3 else 50000
        alpha = "ab" if random.random() < 0.7 else "abcde"
        lines.append("".join(random.choice(alpha) for _ in range(n)))
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
