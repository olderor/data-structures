
// http://codeforces.com/problemset/problem/538/F

#include <iostream> 
#include <fstream> 
#include <vector> 
#include <map>
#include <string>

typedef unsigned long long ull;




int main() {
    /*
    std::ifstream cin("input.txt");
    std::ofstream cout("output.txt");
    */

    int n;
    std::cin >> n;

    std::vector<int> answer(n);
    std::vector<int> data(n);


    for (int i = 0; i < n; ++i) {
        std::cin >> data[i];
    }

    for (int i = 1; i < n; ++i) {
        int x = 0;
        int left_parent, right_parent;
        for (; x * x < i; ++x) {
            right_parent = (i - 1) / (x + 1);
            if (x == 0) {
                left_parent = n - 1;
            } else {
                left_parent = (i - 1) / x;
            }

            if (data[i] < data[x]) {
                ++answer[right_parent];
                --answer[left_parent];
            }
        }

        for (int k = 1; k < (i - 1) / x + 1; ++k) {
            int parent = (i - 1) / k;
            if (data[i] < data[parent]) {
                ++answer[k - 1];
                --answer[k];
            }
        }
    }

    int s = 0;
    for (int i = 0; i < n - 1; ++i) {
        s += answer[i];
        std::cout << s << " ";
    }
    /*
    cin.close();
    cout.close();
    */
    return 0;
}
