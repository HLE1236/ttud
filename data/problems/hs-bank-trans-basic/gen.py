import sys, os, random
random.seed(511)
tdir = sys.argv[1]
cases = [
    "T000010010 T000010020 1000 10:20:30 ATM1\nT000010010 T000010030 2000 10:02:30 ATM2\nT000010010 T000010040 1500 09:23:30 ATM1\nT000010020 T000010030 3000 08:20:31 ATM1\nT000010030 T000010010 4000 12:40:00 ATM2\nT000010040 T000010010 2000 10:30:00 ATM1\nT000010020 T000010040 3000 08:20:31 ATM1\nT000010040 T000010030 2000 11:30:00 ATM1\nT000010040 T000010030 1000 18:30:00 ATM1\n#\n?number_transactions\n?total_money_transaction\n?list_sorted_accounts\n?total_money_transaction_from T000010010\n?inspect_cycle T000010010 3\n#\n",
]
for idx in range(9):
    nv = 10 if idx < 4 else 40
    ne = 30 if idx < 4 else 300
    accs = [f"A{i:06d}" for i in range(1, nv + 1)]
    lines = []
    for _ in range(ne):
        u, v = random.choice(accs), random.choice(accs)
        lines.append(f"{u} {v} {random.randint(10, 5000)} 10:00:00 ATM1")
    lines.append("#")
    lines.extend(["?number_transactions", "?total_money_transaction", "?list_sorted_accounts"])
    for _ in range(20):
        if random.random() < 0.5:
            lines.append(f"?total_money_transaction_from {random.choice(accs)}")
        else:
            lines.append(f"?inspect_cycle {random.choice(accs)} {random.randint(2, 5)}")
    lines.append("#")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
