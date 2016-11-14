
ull t[400001];

void build(ull a[], ull v, ull tl, ull tr) {
    if (tl == tr)
        t[v] = a[tl];
    else {
        ull tm = (tl + tr) / 2;
        build(a, v * 2, tl, tm);
        build(a, v * 2 + 1, tm + 1, tr);
        t[v] = t[v * 2] + t[v * 2 + 1];
    }
}

ull sum(ull v, ull tl, ull tr, ull l, ull r) {
    if (l > r)
        return 0;
    if (l == tl && r == tr)
        return t[v];
    ull tm = (tl + tr) / 2;
    return sum(v * 2, tl, tm, l, min(r, tm))
        + sum(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
}

void update(ull v, ull tl, ull tr, ull pos, ull new_val) {
    if (tl == tr)
        t[v] = new_val;
    else {
        ull tm = (tl + tr) / 2;
        if (pos <= tm)
            update(v * 2, tl, tm, pos, new_val);
        else
            update(v * 2 + 1, tm + 1, tr, pos, new_val);
        t[v] = t[v * 2] + t[v * 2 + 1];
    }
}
