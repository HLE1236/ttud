import os, sys, random
tdir = sys.argv[1]
random.seed(20250201)
def w(i, s): open(os.path.join(tdir, f"{i:02d}.in"), "w").write(s.strip() + "\n")
w(1, "4 10\n2 1 1 3")
w(2, "2 7\n2 4")
w(3, "1 1\n1")
w(4, "5 100\n1 5 10 25 50")
for idx in range(5, 11):
    n = random.choice([10, 50, 200, 1000])
    x = random.choice([50, 300, 750, 1000])
    coins = [random.randint(1, min(x, 200)) for _ in range(n)]
    if idx == 7:
        coins = [random.randrange(2, 50, 2) for _ in range(n)]
        if x % 2 == 0: x -= 1
    w(idx, f"{n} {x}\n" + " ".join(map(str, coins)))
