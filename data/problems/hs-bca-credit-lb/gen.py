import sys, os, random
random.seed(210)
tdir = sys.argv[1]
sample = "4 12\n5 1 3 5 10 12\n5 9 3 4 8 12\n6 1 2 3 4 9 7\n7 1 2 3 5 6 10 11\n2 3 3 2 1 2 3 2 2 2 3 1\n3 1 1 1\n25\n1 2\n1 3\n1 5\n2 4\n2 5\n2 6\n3 5\n3 7\n3 10\n4 6\n4 9\n5 6\n5 7\n5 8\n6 8\n6 9\n7 8\n7 10\n7 11\n8 9\n8 11\n8 12\n9 12\n10 11\n11 12\n"
cases = [sample]
for idx in range(9):
    m = random.randint(3, 5)
    n = random.randint(8, 16)
    lines = [f"{m} {n}"]
    prefs = []
    for t in range(m):
        cnt = random.randint(max(2, n // 2), n)
        cs = sorted(random.sample(range(1, n + 1), cnt))
        prefs.append(set(cs))
        lines.append(f"{len(cs)} " + " ".join(map(str, cs)))
    # ensure every course has at least 1 teacher
    for c in range(1, n + 1):
        if not any(c in p for p in prefs):
            t = random.randrange(m)
            prefs[t].add(c)
    # rebuild lines with guaranteed coverage
    lines = [f"{m} {n}"]
    for p in prefs:
        cs = sorted(p)
        lines.append(f"{len(cs)} " + " ".join(map(str, cs)))
    lines.append(" ".join(str(random.randint(1, 6)) for _ in range(n)))
    if idx == 8:
        # impossible LB sum > n
        lbs = [n // m + 2] * m
    else:
        lbs = [random.randint(1, max(1, n // (m + 1))) for _ in range(m)]
    lines.append(" ".join(map(str, lbs)))
    all_pairs = [(i, j) for i in range(1, n + 1) for j in range(i + 1, n + 1)]
    confs = random.sample(all_pairs, min(len(all_pairs), random.randint(3, n)))
    lines.append(str(len(confs)))
    for u, v in confs:
        lines.append(f"{u} {v}")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
