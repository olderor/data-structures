
// https://contest.yandex.ru/contest/2923/problems/C/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>


typedef unsigned long long ll;

ll peopleWeights[502];
ll weightsPerOneBus[502][502];
ll optimalWeights[502][502];

int main() {

    std::ifstream cin("c.in");
    std::ofstream cout("c.out");

    int busCount, peopleCount;
    ll busCapacity;
    cin >> busCount >> busCapacity >> peopleCount;

    for (int i = 0; i < peopleCount; ++i) {
        cin >> peopleWeights[i];
    }

    for (int counterI = peopleCount - 1; counterI >= 0; --counterI) {
        std::priority_queue<ll> currentWeights;
        ll currentWeightsSum = 0;
        ll currentWeightsSize = 0;

        for (int counterJ = counterI; counterJ < peopleCount; ++counterJ) {

            if (peopleWeights[counterJ] <= busCapacity - currentWeightsSum) {
                currentWeights.push(peopleWeights[counterJ]);
                currentWeightsSum += peopleWeights[counterJ];
                ++currentWeightsSize;
            } else {
                ll top = currentWeights.top();
                if (top > peopleWeights[counterJ]) {
                    currentWeights.pop();
                    currentWeights.push(peopleWeights[counterJ]);
                    currentWeightsSum += peopleWeights[counterJ] - top;
                }
            }

            weightsPerOneBus[counterI][counterJ] = currentWeightsSize;
        }
    }

    for (int j = 0; j < busCount; ++j) {
        for (int i = peopleCount - 1; i >= 0; --i) {

            optimalWeights[i][j] = std::max(optimalWeights[i][j], weightsPerOneBus[0][i]);
            for (int k = 0; k < i; ++k) {
                optimalWeights[i][j] = std::max(optimalWeights[i][j], 
                    optimalWeights[k][j - 1] + weightsPerOneBus[k + 1][i]);
            }
        }
    }

    cout << optimalWeights[peopleCount - 1][busCount - 1];

    cin.close();
    cout.close();

    return 0;
}
