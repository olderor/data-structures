
// http://nure.dots.org.ua/problems?id=2131

#include <iostream> 
#include <fstream> 
#include <vector> 
#include <map>
#include <string>

typedef unsigned long long ull;

ull pow_two(int st) {
    ull res = 1;
    for (int i = 0; i < st; ++i) {
        res *= 2;
    }
    return res;
}

ull get_sum(std::vector< std::vector<unsigned int> > &as, int index) {
    ull sum = 0;
    int asi = 0;
    while (index != 0) {
        int size = as[asi].size();
        if (index < size) {
            return sum + as[asi][index];
        }
        sum += as[asi][size - 1];
        ++asi;
        index -= size - 1;
    }
    return sum;
}

ull get_sum2(std::vector<ull> &as, int index) {
    if (index == 0)
        return 0;
    --index;
    return as[index % as.size()] + (index / as.size()) * as[as.size() - 1];
}

int main() {

    std::ifstream cin("input.txt");
    std::ofstream cout("output.txt");

    std::vector<ull> as;
    int n;
    int x, y;
    ull a0;
    cin >> n >> x >> y >> a0;
    const ull mod = pow_two(16);
    const ull mod2 = pow_two(30);

    ull prev = 0;
    for (int i = 1; i <= n; ++i) {
        if (i > 1 && a0 == as[0]) {
            break;
        }
        as.push_back(prev + a0);
        prev = as[as.size() - 1];
        a0 = (x * a0 + y) % mod;
    }

    int m, z, t;
    ull b0;
    ull sum = 0;
    cin >> m >> z >> t >> b0;

    for (int i = 0; i < m; ++i) {
        ull b1 = (z * b0 + mod2 + t) % mod2;
        int c1 = b1 % n;
        int c0 = b0 % n;
        ull t1, t2;
        if (c1 < c0) {
            t1 = get_sum2(as, c0 + 1);
            t2 = get_sum2(as, c1);
        } else {
            t1 = get_sum2(as, c1 + 1);
            t2 = get_sum2(as, c0);
        }
        sum += t1 - t2;
        b0 = b1;
        b0 = (z * b0 + mod2 + t) % mod2;
    }
   

    cout << sum;

    cin.close();
    cout.close();

    return 0;
}
