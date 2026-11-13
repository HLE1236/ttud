import sys

def check(inp_text, exp_text, act_text):
    tokens = list(map(int, inp_text.split()))
    it = iter(tokens)
    n = next(it)
    d = [[0]*(n+1) for _ in range(n+1)]
    for i in range(1, n+1):
        for j in range(1, n+1):
            d[i][j] = next(it)
    m = next(it)
    pairs = [(next(it), next(it)) for _ in range(m)]

    act_tokens = act_text.strip().split()
    if not act_tokens:
        return False, "Output rong"
    try:
        nums = list(map(int, act_tokens))
    except ValueError:
        return False, "Output chua ky tu khong phai so"
    nx = nums[0]
    if nx != n or len(nums) != n + 1:
        return False, f"Can in {n} va dung {n} dinh (hien co nx={nx}, len={len(nums)-1})"
    tour = nums[1:]
    if sorted(tour) != list(range(1, n+1)):
        return False, "Hanh trinh khong phai hoan vi hop le cua 1..n"
    pos = {v: i for i, v in enumerate(tour)}
    for u, v in pairs:
        if pos[u] > pos[v]:
            return False, f"Vi pham thu tu: {u} phai di truoc {v}"
    cost_act = sum(d[tour[i]][tour[(i+1)%n]] for i in range(n))
    exp_nums = list(map(int, exp_text.strip().split()))
    # Note: In official zip tc1, output line1 had 9 (cost), line2 had 5 nodes. Check if len == n+1
    if len(exp_nums) == n + 1:
        exp_tour = exp_nums[1:]
        if sorted(exp_tour) == list(range(1, n+1)):
            cost_exp = sum(d[exp_tour[i]][exp_tour[(i+1)%n]] for i in range(n))
            return True, f"Hop le! Tong quang duong = {cost_act} (tham khao: {cost_exp})"
    return True, f"Hop le! Tong quang duong = {cost_act}"
