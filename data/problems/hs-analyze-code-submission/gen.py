import sys, os, random
random.seed(512)
tdir = sys.argv[1]
cases = [
    "U001 P01 10:30:20 ERR 0\nU001 P01 10:35:20 OK 10\nU001 P02 10:40:20 ERR 0\nU001 P02 10:55:20 OK 7\nU002 P01 10:40:20 ERR 0\nU001 P01 11:35:20 OK 8\nU002 P02 10:40:20 OK 10\n#\n?total_number_submissions\n?number_error_submision\n?number_error_submision_of_user U002\n?total_point_of_user U001\n?number_submission_period 10:00:00 11:30:45\n#\n",
]
def rtime():
    return f"{random.randint(0,23):02d}:{random.randint(0,59):02d}:{random.randint(0,59):02d}"
for idx in range(9):
    n = 50 if idx < 4 else (3000 if idx < 7 else 50000)
    q = 50 if idx < 4 else (3000 if idx < 7 else 50000)
    users = [f"U{i:04d}" for i in range(1, 200)]
    probs = [f"P{i:02d}" for i in range(1, 15)]
    lines = []
    for _ in range(n):
        st = "ERR" if random.random() < 0.35 else "OK"
        pt = 0 if st == "ERR" else random.randint(1, 10)
        lines.append(f"{random.choice(users)} {random.choice(probs)} {rtime()} {st} {pt}")
    lines.append("#")
    for _ in range(q):
        tp = random.randint(1, 5)
        if tp == 1: lines.append("?total_number_submissions")
        elif tp == 2: lines.append("?number_error_submision")
        elif tp == 3: lines.append(f"?number_error_submision_of_user {random.choice(users)}")
        elif tp == 4: lines.append(f"?total_point_of_user {random.choice(users)}")
        else:
            t1, t2 = sorted([rtime(), rtime()])
            lines.append(f"?number_submission_period {t1} {t2}")
    lines.append("#")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
