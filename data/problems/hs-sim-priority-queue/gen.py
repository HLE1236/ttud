import sys, os, random
tdir = sys.argv[1]; rng = random.Random(10999)
cases = ["PUSH 10\nPUSH 4\nPUSH 1\nPUSH 8\nPUSH 7\nPOP\nPUSH 2\nPUSH 9\nPOP\nPOP\nPOP\n#\n"]
for ops in [20, 50, 200, 1000, 5000, 10000, 25000, 50000, 100000]:
    lines = []; sz = 0
    for _ in range(ops):
        if sz == 0 or rng.random() < 0.65:
            lines.append(f"PUSH {rng.randint(1, 10**6)}"); sz += 1
        else:
            lines.append("POP"); sz -= 1
    lines.append("#")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
