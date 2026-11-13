import sys, os, random
tdir = sys.argv[1]; rng = random.Random(20205)
def fmt(s): return f"{s//3600:02d}:{(s%3600)//60:02d}:{s%60:02d}"
cases = ["ORD0001 18:48:34\nORD0002 15:53:51\nORD0003 08:07:12\nORD0004 04:06:44\nORD0005 05:11:40\nORD0006 00:18:17\nORD0007 05:09:07\nORD0008 18:48:34\nORD0009 02:31:11\nORD0010 18:48:34\n#\n?number_orders\n?number_orders_in_period 03:00:00 06:30:45\n?number_orders_at_time 18:48:34\n###\n"]
for n, q in [(20, 15), (100, 50), (500, 200), (2000, 1000), (10000, 5000), (30000, 10000), (50000, 20000), (80000, 50000), (100000, 50000)]:
    lines = []; ts = []
    for i in range(n):
        s = rng.randrange(86400); ts.append(s)
        lines.append(f"ORD{i+1:05d} {fmt(s)}")
    lines.append("#")
    for _ in range(q):
        tp = rng.randrange(3)
        if tp == 0: lines.append("?number_orders")
        elif tp == 1:
            a, b = sorted([rng.randrange(86400), rng.randrange(86400)])
            lines.append(f"?number_orders_in_period {fmt(a)} {fmt(b)}")
        else:
            lines.append(f"?number_orders_at_time {fmt(rng.choice(ts))}")
    lines.append("###")
    cases.append("\n".join(lines) + "\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
