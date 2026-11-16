import sys, os, random
random.seed(109)
tdir = sys.argv[1]
cases = [
    "computer\nThere are so many kinds of computers today including high performance computers, laptop computers. Mobile phones are also computers\n",
    "aa\naaaaaa\n",
    "aba\nabababa\n",
    "xyz\nabcdefgh\n",
    "a" * 5000 + "b\n" + "a" * 500000 + "\n",
    "a" * 1000 + "\n" + "a" * 200000 + "\n",
]
for idx in range(4):
    m = random.randint(5, 50)
    n = 100000 if idx >= 2 else 1000
    p = "".join(random.choice("ab") for _ in range(m))
    t = "".join(random.choice("ab") for _ in range(n))
    cases.append(f"{p}\n{t}\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
