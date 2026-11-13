import sys
in_p, exp_p, out_p = sys.argv[1], sys.argv[2], sys.argv[3]
n, k = map(int, open(in_p).read().split())
tokens = open(out_p).read().split()
if len(tokens) != k * (n + 1):
    print(f"Expected {k*(n+1)} tokens, got {len(tokens)}")
    sys.exit(1)
seen = set()
pos = 0
for s_idx in range(k):
    nn = int(tokens[pos]); pos += 1
    if nn != n:
        print(f"Solution {s_idx+1}: expected n={n}, got {nn}")
        sys.exit(1)
    arr = [int(x) for x in tokens[pos:pos+n]]; pos += n
    if sorted(arr) != list(range(1, n + 1)):
        print(f"Solution {s_idx+1}: not a valid permutation 1..{n}")
        sys.exit(1)
    d1, d2 = set(), set()
    for c, r in enumerate(arr, 1):
        if (c - r) in d1 or (c + r) in d2:
            print(f"Solution {s_idx+1}: diagonal conflict at col {c}, row {r}")
            sys.exit(1)
        d1.add(c - r); d2.add(c + r)
    t = tuple(arr)
    if t in seen:
        print(f"Duplicate solution found at index {s_idx+1}")
        sys.exit(1)
    seen.add(t)
sys.exit(0)
