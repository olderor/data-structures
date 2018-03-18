#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>
using namespace std;

int main() {
  const int N = 100000;
	std::vector<int> v(N);
  
	std::iota(v.begin(), v.end(), 0);
  
	for (auto i = 0; i < v.size(); ++i) {
		int pivot = (v.size() - 1 + i) / 2;
		v[v[pivot]] = i;
		std::swap(v[pivot], v[i]);
	}
	cout << v.size() << endl;
	for (int i = 0;i < v.size() - 1;i++) {
		cout << v[i] << " ";
	}
	cout << v[v.size() - 1] << endl;
	return 0;
}
