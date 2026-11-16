import sys, os, random
random.seed(513)
tdir = sys.argv[1]
cases = [
    "Q001 2\nQ002 3\nQ003 5\nQ004 1\nQ005 5\n#\nA00000001 Q001 1\nA00000002 Q001 0\nA00000003 Q001 0\nA00000004 Q001 0\nA00000006 Q002 0\nA00000007 Q002 0\nA00000008 Q002 1\nA00000009 Q002 0\nA00000011 Q003 0\nA00000012 Q003 0\nA00000013 Q003 1\nA00000014 Q003 0\nA00000016 Q004 0\nA00000017 Q004 1\nA00000018 Q004 0\nA00000019 Q004 0\nA00000021 Q005 1\nA00000022 Q005 0\nA00000023 Q005 0\nA00000024 Q005 0\n#\nU001 T03 A00000002 03:01:20\nU001 T03 A00000001 12:51:32\nU003 T01 A00000016 12:21:14\nU004 T03 A00000001 06:11:34\nU005 T02 A00000009 19:55:42\nU004 T01 A00000003 21:33:04\nU002 T01 A00000017 04:47:33\nU003 T01 A00000018 19:23:07\nU005 T02 A00000021 11:50:14\nU004 T02 A00000001 14:04:52\n#\n?point_user_of_test U002 T01\n?point_user_of_test U001 T01\n?point_user_of_test U001 T03\n?point_user_of_test U002 T02\n###\n",
]
def rtime():
    return f"{random.randint(0,23):02d}:{random.randint(0,59):02d}:{random.randint(0,59):02d}"
for idx in range(9):
    nq = 10 if idx < 4 else 50
    nlog = 40 if idx < 4 else 5000
    lines = [f"Q{i:03d} {random.randint(1, 10)}" for i in range(1, nq + 1)] + ["#"]
    aids = []
    for i in range(1, nq + 1):
        corr = random.randrange(4)
        for j in range(4):
            aid = f"A{i:03d}_{j}"
            aids.append(aid)
            lines.append(f"{aid} Q{i:03d} {1 if j == corr else 0}")
    lines.append("#")
    users = [f"U{i:03d}" for i in range(1, 50)]
    tests = [f"T{i:02d}" for i in range(1, 6)]
    for _ in range(nlog):
        lines.append(f"{random.choice(users)} {random.choice(tests)} {random.choice(aids)} {rtime()}")
    lines.append("#")
    for _ in range(min(200, nlog)):
        lines.append(f"?point_user_of_test {random.choice(users)} {random.choice(tests)}")
    lines.append("###")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
