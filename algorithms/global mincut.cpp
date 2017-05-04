// https://contest.yandex.ru/contest/4479/problems/

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <utility>
#include <string>
#include <queue>

int vertices_count, edges_count;
std::vector< std::vector<long long> > graph;
std::vector<int> best_cut;
long long best_cost = 9223372036854775807;

std::vector< std::vector<int> > vertices;
std::vector<int> used;
std::vector<int> in_set;
std::vector<long long> set_values;

void mincut() {
    vertices.clear();
    vertices.resize(vertices_count);
    used.clear();
    used.resize(vertices_count);
    for (int i = 0; i < vertices_count; ++i) {
        vertices[i].push_back(i);
    }

    for (int phase = 1; phase < vertices_count; ++phase) {
        in_set.clear();
        in_set.resize(vertices_count);
        set_values.clear();
        set_values.resize(vertices_count);

        int previous;
        for (int phase_i = 0; phase_i <= vertices_count - phase; ++phase_i) {
            int new_vertex = -1;
            for (int i = 0; i < vertices_count; ++i) {
                if (!used[i] && !in_set[i] &&
                    (new_vertex == -1 || set_values[i] > set_values[new_vertex])) {
                    new_vertex = i;
                }
            }

            if (phase_i != vertices_count - phase) {
                in_set[new_vertex] = 1;
                for (int i = 0; i < vertices_count; ++i) {
                    set_values[i] += graph[new_vertex][i];
                }
                previous = new_vertex;
                continue;
            }

            used[new_vertex] = 1;

            if (best_cost > set_values[new_vertex]) {
                best_cost = set_values[new_vertex];
                best_cut = vertices[new_vertex];
            }

            for (int i = 0; i < vertices[new_vertex].size(); ++i) {
                vertices[previous].push_back(vertices[new_vertex][i]);
            }

            for (int i = 0; i < vertices_count; ++i) {
                graph[previous][i] += graph[i][new_vertex];
                graph[i][previous] = graph[previous][i];
            }
        }
    }
}


void run(int index) {
    best_cost = 9223372036854775807;
    best_cut.clear();

    scanf("%d%d", &vertices_count, &edges_count);
    graph.clear();
    graph.resize(vertices_count);
    for (int i = 0; i < vertices_count; ++i) {
        graph[i].resize(vertices_count);
    }

    int from, to;
    long long cost;
    for (int i = 0; i < edges_count; ++i) {
        scanf("%d%d", &from, &to);
        scanf("%lld", &cost);
        graph[from - 1][to - 1] += cost;
        graph[to - 1][from - 1] += cost;
    }
    mincut();

    int size = best_cut.size();
    printf("%lld\n", best_cost);
    printf("%d\n", size);
    for (int i = 0; i < size; ++i) {
        printf("%d ", best_cut[i] + 1);
    }
}

int main()
{
    run(0);
    return 0;
}
