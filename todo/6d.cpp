#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

#pragma warning(disable: 4996)

long long INF = 9223372036854775807;

int costs[501][501];
long long flows[501][501];
std::vector<int> shortest_path;
std::vector<int> available_vertices;

int vertices_count, edges_count;

int e1dges[10001];
int e2dges[10001];
long long minf;
long long delta;

long long dfs(const int vertex, const long long flow) {
    if (vertex == vertices_count - 1) {
        return flow;
    }

    if (flow == 0) {
        return 0;
    }

    int to_vertex = available_vertices[vertex];
    while (to_vertex < vertices_count) {
        if (shortest_path[vertex] + 1 != shortest_path[to_vertex]) {
            ++to_vertex;
            continue;
        }

        minf = flow;
        if (minf > costs[vertex][to_vertex] - flows[vertex][to_vertex]) {
            minf = costs[vertex][to_vertex] - flows[vertex][to_vertex];
        }
        delta = dfs(
            to_vertex,
            minf);

        if (delta != 0) {
            flows[vertex][to_vertex] += delta;
            flows[to_vertex][vertex] -= delta;
            available_vertices[vertex] = to_vertex;
            return delta;
        }
        ++to_vertex;
    }
    available_vertices[vertex] = to_vertex;

    return 0;
}

bool bfs() {
    int vertices_queue[501];
    vertices_queue[0] = 0;
    int size = 1;

    shortest_path = std::vector<int>(vertices_count, -1);

    shortest_path[0] = 0;

    for (int vertex_index = 0; vertex_index < size; ++vertex_index) {
        int vertex = vertices_queue[vertex_index];
        for (int to_vertex = 0; to_vertex < vertices_count; ++to_vertex) {
            if (shortest_path[to_vertex] == -1 &&
                flows[vertex][to_vertex] < costs[vertex][to_vertex]) {
                vertices_queue[size++] = to_vertex;
                shortest_path[to_vertex] = shortest_path[vertex] + 1;
            }
        }
    }
    return shortest_path[vertices_count - 1] != -1;
}

long long dinic() {
    long long flow = 0;
    while (bfs()) {
        available_vertices = std::vector<int>(vertices_count, 0);
        delta = dfs(0, INF);
        while (delta != 0) {
            flow += delta;
            delta = dfs(0, INF);
        }
        flow += delta;
    }
    return flow;
}

int main() {
    // std::cin >> vertices_count >> edges_count;
    scanf("%d", &vertices_count);

    int cost;
    for (int indexi = 0; indexi < vertices_count; ++indexi) {
        scanf("%d", &costs[0][indexi + 1]);
        for (int i = 0; i < vertices_count; ++i) {
            costs[indexi + 1][vertices_count + i + 1] = 200;
            // costs[vertices_count + i + 1][indexi + 1] = 200;
        }
        // std::cin >> vertex_first >> vertex_second >> cost;
    }
    long long res = 0;
    for (int indexi = 0; indexi < vertices_count; ++indexi) {
        scanf("%d", &costs[vertices_count + indexi + 1][vertices_count * 2 + 1]);
        res += costs[vertices_count + indexi + 1][vertices_count * 2 + 1];
        // std::cin >> vertex_first >> vertex_second >> cost;
    }

    vertices_count = vertices_count * 2 + 2;

    long long flow = dinic();
    if (res == flow) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }

    return 0;
}
