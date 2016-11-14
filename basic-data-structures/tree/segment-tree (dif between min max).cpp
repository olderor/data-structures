
const int n = 100001;
int t_max[4 * n + 4];
int t_min[4 * n + 4];


void build(int v, int tl, int tr) {
    if (tl == tr) {
        long long i = tl + 1;
        t_max[v] = ((i * i) % 12345) + (((i * i) % 23456) * i) % 23456;
        t_min[v] = t_max[v];
    } else {
        int tm = (tl + tr) / 2;
        build(v * 2, tl, tm);
        build(v * 2 + 1, tm + 1, tr);
        t_max[v] = max(t_max[v * 2], t_max[v * 2 + 1]);
        t_min[v] = min(t_min[v * 2], t_min[v * 2 + 1]);
    }
}

int get_min(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return n;
    if (l == tl && r == tr)
        return t_min[v];
    int tm = (tl + tr) / 2;
    return min(get_min(v * 2, tl, tm, l, min(r, tm)),
        get_min(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r));
}

int get_max(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return -n;
    if (l == tl && r == tr)
        return t_max[v];
    int tm = (tl + tr) / 2;
    return max(get_max(v * 2, tl, tm, l, min(r, tm)),
        get_max(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r));
}

void update(int v, int tl, int tr, int pos, int new_val) {
    if (tl == tr) {
        t_max[v] = new_val;
        t_min[v] = new_val;
    }
    else {
        int tm = (tl + tr) / 2;
        if (pos <= tm)
            update(v * 2, tl, tm, pos, new_val);
        else
            update(v * 2 + 1, tm + 1, tr, pos, new_val);
        t_max[v] = max(t_max[v * 2], t_max[v * 2 + 1]);
        t_min[v] = min(t_min[v * 2], t_min[v * 2 + 1]);
    }
}
