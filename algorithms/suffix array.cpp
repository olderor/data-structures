// https://contest.yandex.ru/contest/4612/problems/A/

//
//  main.cpp
//  advanced5-1
//
//  Created by olderor on 17.06.17.
//  Copyright © 2017 Bohdan Yevchenko. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::string str;

bool str_cmp(const int first, const int second) {
    return str[first] < str[second];
}

std::vector<int> getSuffixArray() {
    const int length = static_cast<int>(str.length());
    std::vector<int> suffix_array(length);
    std::vector<int> suffix_indexes(length);
    std::vector<int> counts(length);
    std::vector<int> nexts(length);
    std::vector<bool> equal_starts(length);
    std::vector<bool> current_equal_starts(length);

    for (int i = 0; i < length; ++i) {
        suffix_array[i] = i;
    }
    sort(suffix_array.begin(), suffix_array.end(), str_cmp);

    for (int i = 0; i < length; ++i) {
        equal_starts[i] =
          i == 0 || str[suffix_array[i - 1]] != str[suffix_array[i]];
        current_equal_starts[i] = false;
    }

    for (int len = 1; len < length; len <<= 1) {
        int index;
        for (int i = 0; i < length; i = index) {
            index = i + 1;
            while (index < length && !equal_starts[index]) {
                ++index;
            }
            nexts[i] = index;
        }

        for (int i = 0; i < length; i = nexts[i]) {
            for (int j = i; j < nexts[i]; ++j) {
                counts[j] = 0;
                suffix_indexes[suffix_array[j]] = i;
            }
        }

        int suffix = suffix_indexes[length - len];
        current_equal_starts[suffix] = true;
        ++counts[suffix];
        for (int i = 0; i < length; i = nexts[i]) {
            for (int j = i; j < nexts[i]; ++j) {
                suffix = suffix_array[j] - len;
                if (suffix >= 0) {
                    suffix_indexes[suffix] += counts[suffix_indexes[suffix]]++;
                    current_equal_starts[suffix_indexes[suffix]] = true;
                }
            }
            for (int j = i; j < nexts[i]; ++j) {
                suffix = suffix_array[j] - len;
                if (suffix >= 0 &&
                  current_equal_starts[suffix_indexes[suffix]]) {
                    for (index = suffix_indexes[suffix] + 1;
                         !equal_starts[index] && current_equal_starts[index];
                         ++index) {
                        current_equal_starts[index] = false;
                    }
                }
            }
        }
        for (int i = 0; i < length; ++i) {
            equal_starts[i] = equal_starts[i] || current_equal_starts[i];
            suffix_array[suffix_indexes[i]] = i;
        }
    }
    return suffix_array;
}

int main() {
    getline(std::cin, str);
    std::vector<int> suffix_array = getSuffixArray();
    for (int i = 0; i < str.length(); ++i) {
        std::cout << suffix_array[i] + 1 << " ";
    }
    return 0;
}
