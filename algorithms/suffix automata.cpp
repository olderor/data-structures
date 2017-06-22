// https://contest.yandex.ru/contest/4612/problems/B/

//
//  main.cpp
//  advanced5-2
//
//  Created by olderor on 17.06.17.
//  Copyright © 2017 Bohdan Yevchenko. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

struct state {
    int len, link;
    std::vector<int> next;
    state(const int len, const int link) : len(len), link(link) {
        next.resize('z' - 'a' + 2);
    }
};

struct edge {
    int from, to;
    char character;
    edge(const int from, const int to, const char character)
      : from(from), to(to), character(character) {
    }
};

std::vector<state> states;
int previous = 0;

void add(const int char_code) {
    int cur = static_cast<int>(states.size());
    states.push_back(state(states[previous].len + 1, -1));
    
    int state_with_path = previous;
    while (
      state_with_path != -1 && states[state_with_path].next[char_code] == 0) {
        states[state_with_path].next[char_code] = cur;
        state_with_path = states[state_with_path].link;
    }
    
    if (state_with_path == -1) {
        previous = cur;
        states[previous].link = 0;
        return;
    }

    int next = states[state_with_path].next[char_code];
    if (states[next].len == states[state_with_path].len + 1) {
        previous = cur;
        states[previous].link = next;
        return;
    }

    int clone = static_cast<int>(states.size());
    states.push_back(state(states[state_with_path].len + 1, states[next].link));
    states[clone].next = states[next].next;
    while (state_with_path != -1 &&
      states[state_with_path].next[char_code] == next) {
        states[state_with_path].next[char_code] = clone;
        state_with_path = states[state_with_path].link;
    }
    previous = cur;
    states[previous].link = clone;
    states[next].link = clone;
}

int main() {
    states.push_back(state(0, -1));

    std::string str;
    getline(std::cin, str);
    for (int i = 0; i < str.length(); ++i) {
        add(str[i] - 'a');
    }
    
    std::vector<edge> edges;
    std::vector<int> terminates;
    for (int i = 0; i < states.size(); ++i) {
        for (int j = 0; j < states[i].next.size(); ++j) {
            if (states[i].next[j] != 0) {
                edges.push_back(edge(i + 1, states[i].next[j] + 1, 'a' + j));
            }
        }
    }

    while (previous != -1) {
        terminates.push_back(previous + 1);
        previous = states[previous].link;
    }

    printf("%lu %lu\n", states.size(), edges.size());
    for (int i = 0; i < edges.size(); ++i) {
        printf("%d %d %c\n", edges[i].from, edges[i].to, edges[i].character);
    }
    printf("%lu\n", terminates.size());
    for (int i = 0; i < terminates.size(); ++i) {
        printf("%d ", terminates[i]);
    }

    return 0;
}
