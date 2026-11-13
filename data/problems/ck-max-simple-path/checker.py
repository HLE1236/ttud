import sys

def path_weight(n, edge_w, seq):
    if not seq or len(set(seq)) != len(seq):
        return None, "Duplicate or empty vertices"
    for v in seq:
        if v < 1 or v > n:
            return None, f"Vertex out of range: {v}"
    tot = 0
    for i in range(len(seq) - 1):
        u, v = seq[i], seq[i + 1]
        if (u, v) not in edge_w:
            return None, f"Missing edge ({u}, {v})"
        tot += edge_w[(u, v)]
    return tot, "OK"

def main():
    in_path, exp_path, act_path = sys.argv[1], sys.argv[2], sys.argv[3]
    toks = list(map(int, open(in_path, encoding="utf-8").read().split()))
    n, m = toks[0], toks[1]
    edge_w = {}
    idx = 2
    for _ in range(m):
        u, v, w = toks[idx], toks[idx+1], toks[idx+2]
        idx += 3
        edge_w[(u, v)] = max(edge_w.get((u, v), 0), w)
        edge_w[(v, u)] = max(edge_w.get((v, u), 0), w)

    exp_t = list(map(int, open(exp_path, encoding="utf-8").read().split()))
    act_raw = open(act_path, encoding="utf-8").read().split()
    try:
        act_t = list(map(int, act_raw))
    except Exception:
        print("Non-integer output")
        sys.exit(1)

    if len(act_t) < 2 or act_t[0] != len(act_t) - 1:
        print("Invalid output length")
        sys.exit(1)

    exp_w, _ = path_weight(n, edge_w, exp_t[1:])
    act_w, msg = path_weight(n, edge_w, act_t[1:])
    if act_w is None:
        print(msg)
        sys.exit(1)
    if act_w != exp_w:
        print(f"Suboptimal weight: expected {exp_w}, got {act_w}")
        sys.exit(1)
    sys.exit(0)

if __name__ == "__main__":
    main()
