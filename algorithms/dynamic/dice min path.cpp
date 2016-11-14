
// https://contest.yandex.ru/contest/2923/problems/D/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <map>

int dynamic1[1002][6][6][6];
int dynamic2[1002][6][6][6];

int main() {

    std::ifstream cin("d.in");
    std::ofstream cout("d.out");

    int width, height;
    cin >> width >> height;

    int cubeTop, cubeLeft, cubeUp;
    cin >> cubeTop >> cubeLeft >> cubeUp;
    --cubeTop;
    --cubeLeft;
    --cubeUp;

    dynamic2[0][cubeTop][cubeLeft][cubeUp] = cubeTop + 1;

    for (int widthI = 0; widthI < width; ++widthI) {

        for (int heightI = 0; heightI < height; ++heightI) {
            for (int cTop = 0; cTop < 6; ++cTop) {
                for (int cLeft = 0; cLeft < 6; ++cLeft) {
                    for (int cUp = 0; cUp < 6; ++cUp) {
                        dynamic1[heightI][cTop][cLeft][cUp] = dynamic2[heightI][cTop][cLeft][cUp];
                        dynamic2[heightI][cTop][cLeft][cUp] = 0;
                    }
                }
            }
        }


        for (int heightI = 0; heightI < height; ++heightI) {
            for (int cTop = 0; cTop < 6; ++cTop) {
                for (int cLeft = 0; cLeft < 6; ++cLeft) {
                    for (int cUp = 0; cUp < 6; ++cUp) {
                        if (dynamic1[heightI][cTop][cLeft][cUp] == 0) continue;

                        dynamic2[heightI][cLeft][5 - cTop][cUp] = std::min(
                            dynamic2[heightI][cLeft][5 - cTop][cUp],
                            dynamic1[heightI][cTop][cLeft][cUp] + cLeft + 1);

                        if (dynamic2[heightI][cLeft][5 - cTop][cUp] == 0)
                            dynamic2[heightI][cLeft][5 - cTop][cUp] = 
                            dynamic1[heightI][cTop][cLeft][cUp]
                            + cLeft + 1;

                        dynamic1[heightI + 1][cUp][cLeft][5 - cTop] = std::min(
                            dynamic1[heightI + 1][cUp][cLeft][5 - cTop],
                            dynamic1[heightI][cTop][cLeft][cUp] + cUp + 1);

                        if (dynamic1[heightI + 1][cUp][cLeft][5 - cTop] == 0)
                            dynamic1[heightI + 1][cUp][cLeft][5 - cTop] = 
                            dynamic1[heightI][cTop][cLeft][cUp]
                            + cUp + 1;
                    }
                }
            }
        }
    }

    int minAnswer = 2147483646;

    for (int cTop = 0; cTop < 6; ++cTop) {
        for (int cLeft = 0; cLeft < 6; ++cLeft) {
            for (int cUp = 0; cUp < 6; ++cUp) {
                if (dynamic1[height - 1][cTop][cLeft][cUp] != 0 && 
                    dynamic1[height - 1][cTop][cLeft][cUp] < minAnswer)
                    minAnswer = dynamic1[height - 1][cTop][cLeft][cUp];
            }
        }
    }

    cout << minAnswer;

    return 0;
}
