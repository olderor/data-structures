
// https://contest.yandex.ru/contest/2923/problems/B/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>


std::vector<char> dynamic[52];

int main() {

    std::ifstream cin("b.in");
    std::ofstream cout("b.out");

    std::string text, pattern;
    std::getline(cin, text);
    std::getline(cin, pattern);
    int difference;
    cin >> difference;

    int textLength = text.length();
    int patternLength = pattern.length();

    for (int i = 0; i < 52; ++i) {
        for (int j = 0; j <= patternLength + 1; ++j) {
            dynamic[i].push_back(0);
        }
    }
    for (int i = 0; i <= patternLength; ++i) {
        dynamic[0][i] = i;
    }

    int offset = 0;
    for (int textIndex = 1; textIndex + offset <= textLength; ++textIndex) {
        for (int patternIndex = 1; patternIndex <= patternLength; ++patternIndex) {
            int addDifference = dynamic[textIndex - 1][patternIndex] + 1;
            int deleteDifference = dynamic[textIndex][patternIndex - 1] + 1;
            int changeDifference = dynamic[textIndex - 1][patternIndex - 1];
            if (pattern[patternIndex - 1] != text[textIndex + offset - 1]) {
                ++changeDifference;
            }
            dynamic[textIndex][patternIndex] = std::min(addDifference, 
                std::min(deleteDifference, changeDifference));
        }


        if (dynamic[textIndex][patternLength] <= difference) {
            int endIndex = textIndex;
            int startIndex = textIndex;
            int index = patternLength;
            while (index != 0 && startIndex != 0) {
                int addDifference = dynamic[startIndex - 1][index] + 1;
                int deleteDifference = dynamic[startIndex][index - 1] + 1;
                int changeDifference = dynamic[startIndex - 1][index - 1];
                if (pattern[index - 1] != text[startIndex + offset - 1]) {
                    ++changeDifference;
                }
                if (addDifference <= deleteDifference) {
                    --startIndex;
                    if (addDifference > changeDifference) {
                        --index;
                    }
                    continue;
                }

                --index;
                if (deleteDifference > changeDifference) {
                    --startIndex;
                }
            }
            cout << startIndex + offset << " " << endIndex - startIndex;
            return 0;
        }
        if (textIndex == 51) {
            for (int i = 1; i <= textIndex; ++i) {
                dynamic[i].swap(dynamic[i - 1]);
            }
            for (int i = 0; i <= patternLength + 1; ++i) {
                dynamic[textIndex][i] = 0;
            }
            ++offset;
            --textIndex;
        }
    }
    cout << 0 << " " << 0;
    return 0;
}
