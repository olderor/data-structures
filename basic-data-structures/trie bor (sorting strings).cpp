// Copyright 2017 olderor (Yevchenko Bohdan)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#pragma warning(disable:4996)

struct vertex {
    int childs['z' - 'a' + 1];
    int leaf;
    int size;
};

std::vector<vertex> trie;

void add_string(const std::string &str) {
    int current_vertex = 0, char_code;
    for (int i = 0; i < str.length(); ++i) {
        char_code = str[i] - 'a';
        if (!trie[current_vertex].childs[char_code]) {
            trie[current_vertex].childs[char_code] = trie.size();
            trie.push_back(vertex());
        }
        ++trie[current_vertex].size;
        current_vertex = trie[current_vertex].childs[char_code];
    }
    ++trie[current_vertex].leaf;
}

void find(int index) {
    int current_vertex = 0, child;
    std::string answ = "";
    for (int child_i = 0;child_i < 'z' - 'a' + 1; ++child_i) {
        child = trie[current_vertex].childs[child_i];
        if (!child) {
            continue;
        }
        index -= trie[child].leaf;
        if (index < 1) {
            answ.push_back('a' + child_i);
            break;
        }
        if (trie[child].size >= index) {
            answ.push_back('a' + child_i);
            current_vertex = child;
            child_i = -1;
            continue;
        }
        index -= trie[child].size;
    }
    std::cout << answ << "\n";
}

int try_atoi(std::string &str) {
    if (str[0] < '0' || str[0] > '9') {
        return -1;
    }
    int val = 0;
    for (int i = 0; i < str.length(); ++i) {
        val = val * 10 + str[i] - '0';
    }
    return val;
}

int main() {
    trie.push_back(vertex());

    int size;
    std::cin >> size;
    std::string command;
    for (int i = 0; i < size; ++i) {
        std::cin >> command;
        int val = try_atoi(command);
        if (val == -1) {
            add_string(command);
        } else {
            find(val);
        }
    }

    return 0;
}
