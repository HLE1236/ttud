import sys, os, random
tdir = sys.argv[1]; rng = random.Random(40803)
cases = ["2 3\n", "1 18446744073709551614\n", "1000000007 999999999\n", "18446744073709551614 18446744073709551614\n"]
for _ in range(6):
    cases.append(f"{rng.randint(2, 2**64 - 2)} {rng.randint(2, 2**64 - 2)}\n")
for idx, c in enumerate(cases, 1): open(os.path.join(tdir, f"{idx:02d}.in"), "w").write(c)
