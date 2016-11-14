#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

// https://contest.yandex.ru/contest/2923/problems/A/

int main() {

    std::ifstream cin("a.in");
    std::ofstream cout("a.out");

    int elementsCount;
    cin >> elementsCount;
    std::vector< std::pair<std::string, std::set<char> > > dict;

    std::string name;
    std::getline(cin, name);

    for (int i = 0; i < elementsCount; ++i) {
        std::string name, line;
        std::getline(cin, name, ' ');
        std::getline(cin, line);

        std::set<char> letters;
        for (int i = 0; i < line.size(); ++i) {
            letters.insert(line[i]);
        }

        dict.push_back(make_pair(name, letters));
    }

    std::map<char, int> word;
    char c;
    double length = 0;
    while (cin >> c) {
        ++word[c];
        ++length;
    }

    bool foundFlag = false;
    for (int i = 0; i < elementsCount; ++i) {
        double notFound = 0;
        for (std::map<char, int>::iterator it = word.begin(); it != word.end(); ++it) {
            if (dict[i].second.find(it->first) == dict[i].second.end()) {
                notFound += it->second;
            }
        }
        if (notFound <= 0.1 * length) {
            foundFlag = true;
            cout << dict[i].first << std::endl;
        }
    }

    if (!foundFlag) {
        cout << "NO SOLUTION" << std::endl;
    }


    cin.close();
    cout.close();

    return 0;
}
