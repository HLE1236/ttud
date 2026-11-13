import sys, os, random
tdir = sys.argv[1]
rng = random.Random(2024124)
cases = [
    "aabaabca\nabbcabc\naababbaabcabcca\n",
    "abc\ndef\nabdecf\n",
    "abc\ndef\nabdefg\n",
    "aaa\naaa\naaaaaa\n",
    "abc\ndef\nfedcba\n",
]
def gen_pair(n, m, make_valid):
    A = [rng.choice("abcd") for _ in range(n)]
    B = [rng.choice("abcd") for _ in range(m)]
    i = j = 0
    C = []
    while i < n or j < m:
        if i < n and (j == m or rng.random() < 0.5):
            C.append(A[i]); i += 1
        else:
            C.append(B[j]); j += 1
    if not make_valid and len(C) >= 2:
        pos = rng.randrange(len(C))
        C[pos] = 'z'
    return "".join(A) + "\n" + "".join(B) + "\n" + "".join(C) + "\n"

for sz, ok in [(50, True), (200, False), (500, True), (800, False), (1000, True)]:
    cases.append(gen_pair(sz, sz, ok))
for idx, c in enumerate(cases, 1):
    with open(os.path.join(tdir, f"{idx:02d}.in"), "w", encoding="utf-8") as f:
        f.write(c)
