import sys, os, random
random.seed(510)
tdir = sys.argv[1]
cases = [
    "C001 P001 10 SHOP001 10:30:10\nC001 P002 30 SHOP001 12:30:10\nC003 P001 40 SHOP002 10:15:20\nC001 P001 80 SHOP002 08:40:10\nC002 P001 130 SHOP001 10:30:10\nC002 P001 160 SHOP003 11:30:20\n#\n?total_number_orders\n?total_revenue\n?revenue_of_shop SHOP001\n?total_consume_of_customer_shop C001 SHOP001\n?total_revenue_in_period 10:00:00 18:40:45\n#\n",
]
def rtime():
    return f"{random.randint(0,23):02d}:{random.randint(0,59):02d}:{random.randint(0,59):02d}"
for idx in range(9):
    n = 50 if idx < 4 else (3000 if idx < 7 else 50000)
    q = 50 if idx < 4 else (3000 if idx < 7 else 50000)
    custs = [f"C{i:04d}" for i in range(1, min(n, 200) + 1)]
    shops = [f"S{i:03d}" for i in range(1, 30)]
    lines = []
    for _ in range(n):
        lines.append(f"{random.choice(custs)} P001 {random.randint(1, 1000)} {random.choice(shops)} {rtime()}")
    lines.append("#")
    for _ in range(q):
        tp = random.randint(1, 5)
        if tp == 1: lines.append("?total_number_orders")
        elif tp == 2: lines.append("?total_revenue")
        elif tp == 3: lines.append(f"?revenue_of_shop {random.choice(shops)}")
        elif tp == 4: lines.append(f"?total_consume_of_customer_shop {random.choice(custs)} {random.choice(shops)}")
        else:
            t1, t2 = sorted([rtime(), rtime()])
            lines.append(f"?total_revenue_in_period {t1} {t2}")
    lines.append("#")
    cases.append("\n".join(lines) + "\n")
for i, c in enumerate(cases, 1):
    open(os.path.join(tdir, f"{i:02d}.in"), "w").write(c)
