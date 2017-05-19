// https://contest.yandex.ru/contest/4545/problems/E/
// Copyright 2017 olderor (Yevchenko Bohdan)

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#pragma warning(disable:4996)

struct vertex {
    int next[2];
    bool leaf;
    int parent_index;
    int parent_code;
    int link;
    int link_next[2];
    bool checked;
    bool free;
    bool fake;
    std::vector<int> linked;
};

std::vector<vertex> trie;

void add_vertex(const int parent, const int parent_code) {
    trie.push_back(vertex());
    trie[trie.size() - 1].parent_index = parent;
    trie[trie.size() - 1].parent_code = parent_code;
    trie[trie.size() - 1].link = -1;
    trie[trie.size() - 1].link_next[0] = -1;
    trie[trie.size() - 1].link_next[1] = -1;
    trie[trie.size() - 1].next[0] = -1;
    trie[trie.size() - 1].next[1] = -1;
}

void init() {
    add_vertex(-1, -1);
}

void add_string(std::string &str) {
    int current_vertex = 0, char_code;
    for (int i = 0; i < str.length(); ++i) {
        if (trie[current_vertex].leaf) {
            return;
        }
        char_code = str[i] - '0';
        if (trie[current_vertex].next[char_code] == -1) {
            trie[current_vertex].next[char_code] = trie.size();
            add_vertex(current_vertex, char_code);
        }
        current_vertex = trie[current_vertex].next[char_code];
    }
    trie[current_vertex].leaf = 1;
}

int go(const int vertex, const int code);

int get_link(const int vertex) {
    if (trie[vertex].link != -1) {
        return trie[vertex].link;
    }

    if (vertex == 0 || trie[vertex].parent_index == 0) {
        trie[vertex].link = 0;
        return 0;
    }
    trie[vertex].link = go(
        get_link(trie[vertex].parent_index), trie[vertex].parent_code);
    return trie[vertex].link;
}

int go(const int vertex, const int code) {
    if (trie[vertex].link_next[code] != -1) {
        return trie[vertex].link_next[code];
    }
    if (trie[vertex].next[code] != -1) {
        trie[vertex].link_next[code] = trie[vertex].next[code];
        return trie[vertex].link_next[code];
    }
    if (vertex == 0) {
        trie[vertex].link_next[code] = 0;
    } else {
        trie[vertex].link_next[code] = go(get_link(vertex), code);
    }
    return trie[vertex].link_next[code];
}

void set_free(const int vertex) {
    if (trie[vertex].leaf) {
        return;
    }
    if (trie[vertex].next[0] != -1) {
        set_free(trie[vertex].next[0]);
    } else {
        trie[vertex].free = 1;
        trie[vertex].next[0] = trie.size();
        add_vertex(vertex, 0);
        trie[trie.size() - 1].fake = 1;
    }
    if (trie[vertex].next[1] != -1) {
        set_free(trie[vertex].next[1]);
    } else {
        trie[vertex].free = 1;
        trie[vertex].next[1] = trie.size();
        add_vertex(vertex, 1);
        trie[trie.size() - 1].fake = 1;
    }
}

void set_links() {
    for (int i = 0; i < trie.size(); ++i) {
        int link = get_link(i);
        trie[link].linked.push_back(i);
    }
}

bool check_if_leaf(const int vertex) {
    if (trie[vertex].next[0] == -1 || trie[vertex].next[1] == -1) {
        return 0;
    }
    if (!trie[trie[vertex].next[0]].leaf) {
        return 0;
    }
    if (!trie[trie[vertex].next[1]].leaf) {
        return 0;
    }
    return 1;
}

std::vector<int> queue;

void add_leaf(const int vertex) {
    if (trie[vertex].checked) {
        return;
    }
    trie[vertex].checked = 1;
    queue.push_back(vertex);
    trie[vertex].leaf = 1;
    if (trie[vertex].parent_index != -1) {
        if (check_if_leaf(trie[vertex].parent_index)) {
            add_leaf(trie[vertex].parent_index);
        }
    }
}

void make_leafs() {
    for (int i = 0; i < trie.size(); ++i) {
        if (trie[i].leaf) {
            add_leaf(i);
        }
    }
    int qi = 0;
    while (qi < queue.size()) {
        int vertex = queue[qi++];
        for (int i = 0; i < trie[vertex].linked.size(); ++i) {
            add_leaf(trie[vertex].linked[i]);
        }
    }
}

int main() {
    init();

    int size;
    std::cin >> size;
    std::string command;
    for (int i = 0; i < size; ++i) {
        std::cin >> command;
        add_string(command);
    }

    int tries = trie.size();
    set_free(0);
    set_links();
    make_leafs();
    if (trie[0].leaf) {
        std::cout << "NO\n";
        return 0;
    }

    for (int i = tries; i < trie.size(); ++i) {
        if (!trie[i].leaf) {
            std::cout << "YES\n";
            return 0;
        }
    }
    std::cout << "NO\n";
    return 0;
}
