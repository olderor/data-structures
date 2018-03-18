#include <fstream>
#include <vector>
 
using namespace std;
 
int main() {
    int n = 100000;
    // ifstream fin("input.txt");
    // fin >> n;
    // fin.close();
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
         a[i] = i + 1;
    }
    for (int i = 2; i < n; ++i) {
        swap(a[i], a[i / 2]);
    }
    
    ofstream fout("input.txt");
    fout << n << "\n";
    for (int i = 0; i < n; ++i) {
        fout << a[i] << " ";
    }
    fout.close();
    return 0;
}
