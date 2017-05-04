// https://contest.yandex.ru/contest/4479/problems/C/

#include <iostream>
#include <iomanip>
#include <vector>

int verices_count, edges_count;

long double inf = 9223372036854775807;
struct edge {
    int from, to;
    long double cost;
    int flow;
    int capacity;
    int id;
    int back;
};

std::vector< std::vector<edge*> > graph;
std::vector<edge*> edges;
std::vector<long double> distances;
std::vector<int> parents;
std::vector<int> answer;
long double res = 0;

void add_edge(int from, int to, int capacity, int cost, int id) {
    edge *first = new edge();
    first->from = from;
    first->to = to;
    first->capacity = capacity;
    first->cost = cost;
    first->id = id;
    first->back = edges.size() + 1;

    edge *second = new edge();
    second->from = to;
    second->to = from;
    second->cost = -cost;
    second->id = id;
    second->back = edges.size();

    edges.push_back(first);
    edges.push_back(second);
    graph[from].push_back(first);
}

void solve(int id) {
    for (int i = 1; i < verices_count; ++i) {
        distances[i] = inf;
        parents[i] = -1;
    }
    parents[0] = -1;
    distances[0] = 0;
    bool found = true;
    while (found) {
        found = false;
        for (int index = 0; index < edges.size(); ++index) {
            if (edges[index]->flow >= edges[index]->capacity) {
                continue;
            }
            if (distances[edges[index]->from] == inf) {
                continue;
            }
            if (distances[edges[index]->to] > distances[edges[index]->from] + edges[index]->cost) {
                distances[edges[index]->to] = distances[edges[index]->from] + edges[index]->cost;
                parents[edges[index]->to] = index;
                found = true;
            }
        }
    }

    if (distances[verices_count - 1] == inf) {
        res = -1;
        return;
    }
    int cur = verices_count - 1;
    while (cur != 0) {
        edges[parents[cur]]->flow += 1;
        edges[edges[parents[cur]]->back]->flow -= 1;
        cur = edges[parents[cur]]->from;
    }
    res += distances[verices_count - 1];
}

int main() {
    long double tests_count;
    std::cin >> verices_count >> edges_count >> tests_count;

    graph.resize(verices_count);
    distances.resize(verices_count);
    parents.resize(verices_count);
    answer.resize(tests_count);

    int from, to;
    long double cost;
    for (int i = 0; i < edges_count; ++i) {
        std::cin >> from >> to >> cost;
        add_edge(from - 1, to - 1, 1, cost, i + 1);
        add_edge(to - 1, from - 1, 1, cost, i + 1);
    }

    for (int i = 0; i < tests_count; ++i) {
        solve(i);
        if (res == -1) {
            std::cout << -1;
            return 0;
        }
    }

    std::cout << std::fixed << std::setprecision(9) << res / tests_count << '\n';
    for (int index = 0; index < tests_count; ++index) {
        answer.clear();
        int cur = 0;
        while (cur != verices_count - 1) {
            for (int i = 0; i < graph[cur].size(); ++i) {
                if (graph[cur][i]->id == -1 || graph[cur][i]->flow != 1) {
                    continue;
                }
                answer.push_back(graph[cur][i]->id);
                graph[cur][i]->id = -1;
                cur = graph[cur][i]->to;
                break;
            }
        }
        std::cout << answer.size();
        for (int i = 0; i < answer.size(); ++i) {
            std::cout << ' ' << answer[i];
        }
        std::cout << '\n';
    }

    return 0;
}
