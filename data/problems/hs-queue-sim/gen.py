import sys, os, random
tdir = sys.argv[1]; rng = random.Random(10203)
cases = [
    "PUSH 1\nPUSH 2\nPUSH 3\nPOP\nPOP\nPUSH 4\nPUSH 5\nPOP\n#\n",
    "PUSH 1\nPOP\nPOP\nPUSH 4\nPOP\n#\n",
    "POP\n#\n",
]
for ops in [20, 100, 500, 2000, 10000, 50000, 100000]:
    lines = [f"PUSH {rng.randint(1, 10**6)}" if rng.random() < 0.55 else "POP" for _ in range(ops)] + ["#"]
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
