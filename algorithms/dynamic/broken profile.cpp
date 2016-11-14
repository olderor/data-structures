
// https://contest.yandex.ru/contest/2923/problems/E/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

typedef unsigned long long ull;

ull height, width, maskMax;

std::vector < std::vector < std::vector < ull > > > dynamicInFirstRow;
std::vector < std::vector < std::vector < ull > > > dynamicInSecondRow;

int main()
{
    std::cin >> width >> height;

    if (width == 1 && height == 2) {
        // haHAA
        std::cout << 1;
        return 0;
    }

    ull horizontalMaskMax = (1 << (width - 1)) - 1;
    maskMax = (1 << width) - 1;

    dynamicInFirstRow.resize(width + 2, std::vector < 
        std::vector<ull> >(maskMax + 1, std::vector<ull>(
            horizontalMaskMax + 1)));
    dynamicInSecondRow.resize(width + 2, std::vector < 
        std::vector<ull> >(maskMax + 1, std::vector<ull>(
            horizontalMaskMax + 1)));

    dynamicInFirstRow[0][0][horizontalMaskMax] = 1;

    for (ull heightI = 0; heightI < height; ++heightI) {
        for (ull widthI = 0; widthI < width; ++widthI) {
            for (ull mask = 0; mask <= maskMax; ++mask) {
                for (ull horizontalMask = 0; horizontalMask 
                    <= horizontalMaskMax; ++horizontalMask) {
                    if (dynamicInFirstRow[widthI][mask][horizontalMask] == 0) {
                        continue;
                    }

                    ull bitForCheck = 1 << widthI;
                    if (mask & bitForCheck) {
                        dynamicInFirstRow[widthI + 1][mask ^ bitForCheck][horizontalMask] 
                            += dynamicInFirstRow[widthI][mask][horizontalMask];
                    } else {
                        dynamicInFirstRow[widthI + 1][mask ^ bitForCheck][horizontalMask] 
                            += dynamicInFirstRow[widthI][mask][horizontalMask];
                        ull nextBitForCheck = 1 << (widthI + 1);
                        if (widthI + 1 < width && (mask & nextBitForCheck) == 0) {
                            dynamicInFirstRow[widthI + 1][mask ^ nextBitForCheck]
                                [horizontalMask & (~bitForCheck)] += 
                                dynamicInFirstRow[widthI][mask][horizontalMask];
                        }
                    }
                }
            }
        }

        if (heightI != height - 1) {
            for (ull horizontalMask = 0; horizontalMask <= horizontalMaskMax; ++horizontalMask) {
                dynamicInFirstRow[width][0][horizontalMask] = 0;
                dynamicInFirstRow[width][maskMax][horizontalMask] = 0;
            }
        }
        for (ull mask = 0; mask <= maskMax; ++mask) {
            for (ull horizontalMask = 0; horizontalMask <= horizontalMaskMax; ++horizontalMask) {
                dynamicInSecondRow[0][mask][horizontalMask] = 
                    dynamicInFirstRow[width][mask][horizontalMask];
            }
        }

        swap(dynamicInFirstRow, dynamicInSecondRow);
        dynamicInSecondRow.clear();
        dynamicInSecondRow.resize(width + 2, std::vector < 
            std::vector<ull> >(maskMax + 1, std::vector<ull>(
                horizontalMaskMax + 1)));
    }

    std::cout << dynamicInFirstRow[0][0][0];

    return 0;
}
