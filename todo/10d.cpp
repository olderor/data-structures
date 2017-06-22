// https://contest.yandex.ru/contest/4612/problems/D/

//
//  main.cpp
//  advanced5-4
//
//  Created by olderor on 20.06.17.
//  Copyright © 2017 Bohdan Yevchenko. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int alphabet_size;
int size;

struct node {
    int from;
    int to;
    int parent;
    std::vector<int> next;
    int link = -1;

    node(const int from, const int to, const int parent)
      : from(from), to(to), parent(parent) {
        next.resize(alphabet_size + 1, -1);
    }
};

std::vector<int> data;

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
          nodes[parent].next[data[index - (nodes[0].to - nodes[0].from)]];
        while (current != -1 &&
          nodes[0].to - nodes[0].from >=
            nodes[current].to - nodes[current].from) {
            nodes[0].from += nodes[current].to - nodes[current].from;
            parent = current;
            current =
              nodes[parent].next[data[index - (nodes[0].to - nodes[0].from)]];
        }

        if (current == -1) {
            nodes[parent].next[data[index]] = static_cast<int>(nodes.size());
            nodes.push_back(node(index, size, parent));
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
            nodes.push_back(node(index, size, first));

            nodes[first].next[data[index]] = second;
            nodes[first]
              .next[data[nodes[current].from + nodes[0].to - nodes[0].from]] =
              current;
            nodes[current].from += nodes[0].to - nodes[0].from;
            nodes[current].parent = first;
            nodes[parent].next[data[index - (nodes[0].to - nodes[0].from)]] =
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

long long best_length = 0;
long long best_count = 0;
int best_end_index = 0;
long long best_points = 0;

long long dfs(const int vertex, const long long length) {
    long long cur_length;
    long long cur_count, cur_points;
    if (nodes[vertex].to == size && vertex != 0) {
        cur_length = length - 1;
        cur_count = 1;
        cur_points = cur_count * cur_length;
        if (cur_points > best_points) {
            best_length = cur_length;
            best_count = cur_count;
            best_end_index = nodes[vertex].to - 1;
            best_points = cur_points;
        }
        return 1;
    }
    cur_count = 0;
    for (int i = 0, next_node; i < nodes[vertex].next.size(); ++i) {
        next_node = nodes[vertex].next[i];
        if (next_node != -1) {
            cur_count += dfs(
              next_node, length + nodes[next_node].to - nodes[next_node].from);
        }
    }
    cur_length = length;
    cur_points = cur_count * cur_length;
    if (cur_points > best_points) {
        best_length = cur_length;
        best_count = cur_count;
        best_end_index = nodes[vertex].to;
        best_points = cur_points;
    }
    return cur_count;
}

int main() {
    std::cin >> size >> alphabet_size;
    ++size;
    data.resize(size);
    for (int i = 0; i < size - 1; ++i) {
        std::cin >> data[i];
    }
    init();
    data[size - 1] = 0;
    for (int i = 0; i < size; ++i) {
        add(i);
    }
    dfs(0, 0);

    std::cout << best_points << "\n" << best_length << "\n";
    for (int i = best_end_index - static_cast<int>(best_length);
         i < best_end_index;
         ++i) {
        std::cout << data[i] << " ";
    }

    return 0;
}
