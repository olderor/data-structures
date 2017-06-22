// https://contest.yandex.ru/contest/4612/problems/C/

//
//  main.cpp
//  advanced5-3
//
//  Created by olderor on 20.06.17.
//  Copyright © 2017 Bohdan Yevchenko. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const char lower_bound = 'a' - 1;

struct node {
    int from;
    int to;
    int index;
    int parent;
    std::vector<int> next;
    int link = -1;

    node(const int from, const int to, const int parent)
      : from(from), to(to), parent(parent) {
        next.resize('z' - 'a' + 2, -1);
    }
};

std::string data;

std::vector<node> nodes;
int parent;

void init() {
    nodes.push_back(node(0, 0, -1));
    parent = 0;
}

void add(const int index) {
    int last_parent = -1;
    while (nodes[0].to - nodes[0].from >= 0) {
        int current =
          nodes[parent]
            .next[data[index - (nodes[0].to - nodes[0].from)] - lower_bound];
        while (current != -1 &&
          nodes[0].to - nodes[0].from >=
            nodes[current].to - nodes[current].from) {
            nodes[0].from += nodes[current].to - nodes[current].from;
            parent = current;
            current =
              nodes[parent].next[data[index - (nodes[0].to - nodes[0].from)] -
                lower_bound];
        }

        if (current == -1) {
            nodes[parent].next[data[index] - lower_bound] =
              static_cast<int>(nodes.size());
            nodes.push_back(
              node(index, static_cast<int>(data.length()), parent));
            if (last_parent != -1) {
                nodes[last_parent].link = parent;
            }
            last_parent = -1;
        } else {
            if (data[nodes[current].from + nodes[0].to - nodes[0].from] ==
              data[index]) {
                if (last_parent != -1) {
                    nodes[last_parent].link = parent;
                }
                break;
            }
            // split.
            int first = static_cast<int>(nodes.size());
            nodes.push_back(node(nodes[current].from,
              nodes[current].from + nodes[0].to - nodes[0].from,
              parent));
            int second = static_cast<int>(nodes.size());
            nodes.push_back(
              node(index, static_cast<int>(data.length()), first));

            nodes[first].next[data[index] - lower_bound] = second;
            nodes[first]
              .next[data[nodes[current].from + nodes[0].to - nodes[0].from] -
                lower_bound] = current;
            nodes[current].from += nodes[0].to - nodes[0].from;
            nodes[current].parent = first;
            nodes[parent]
              .next[data[index - (nodes[0].to - nodes[0].from)] - lower_bound] =
              first;
            if (last_parent != -1) {
                nodes[last_parent].link = first;
            }
            last_parent = first;
        }

        if (parent == 0) {
            ++nodes[0].from;
        } else {
            parent = nodes[parent].link;
        }
    }
    ++nodes[0].to;
}

std::vector<int> answer;

void dfs(const int vertex) {
    nodes[vertex].index = static_cast<int>(answer.size());
    answer.push_back(vertex);
    for (int i = 0; i < nodes[vertex].next.size(); ++i) {
        if (nodes[vertex].next[i] != -1) {
            dfs(nodes[vertex].next[i]);
        }
    }
}

int main() {
    std::cin >> data;
    init();
    data[data.length() - 1] = 'a' - 1;
    for (int i = 0; i < data.length(); ++i) {
        add(i);
    }
    dfs(0);
    std::cout << answer.size() << "\n";
    for (int i = 1; i < answer.size(); ++i) {
        std::cout << nodes[nodes[answer[i]].parent].index << " "
                  << nodes[answer[i]].from << " " << nodes[answer[i]].to
                  << "\n";
    }

    return 0;
}
