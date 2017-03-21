#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

const int MAXN = 101;
const int INF = 10000000 * MAXN;

std::vector< std::vector<int> > costs;
std::vector< std::vector<int> > flows;
std::vector<int> shortest_path;
std::vector<int> available_vertices;
std::vector<int> vertices_queue;

int vertices_count, edges_count;

std::vector<int> best_cut_part_vertices;

std::vector<std::pair<std::pair<int, int>, int>> edges;


int dfs(int vertex, int flow) {
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

        int delta = dfs(
            to_vertex, 
            std::min(
                flow, 
                costs[vertex][to_vertex] - flows[vertex][to_vertex]));

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
    best_cut_part_vertices.clear();
    vertices_queue.clear();
    shortest_path = std::vector<int>(vertices_count, -1);

    vertices_queue.push_back(0);
    shortest_path[0] = 0;

    for (int vertex_index = 0; vertex_index < vertices_queue.size(); ++vertex_index) {
        int vertex = vertices_queue[vertex_index];
        best_cut_part_vertices.push_back(vertex);
        for (int to_vertex = 0; to_vertex < vertices_count; ++to_vertex) {
            if (shortest_path[to_vertex] == -1 &&
                flows[vertex][to_vertex] < costs[vertex][to_vertex]) {
                vertices_queue.push_back(to_vertex);
                shortest_path[to_vertex] = shortest_path[vertex] + 1;
            }
        }
    }
    return shortest_path[vertices_count - 1] != -1;
}

void dinic() {
    while (bfs()) {
        available_vertices = std::vector<int>(vertices_count, 0);
        int delta = dfs(0, INF);
        while (delta != 0) {
            delta = dfs(0, INF);
        }
    }
}

int main() {
    std::cin >> vertices_count >> edges_count;

    costs = std::vector< std::vector<int> >(
        vertices_count, 
        std::vector<int>(vertices_count));

    flows = std::vector< std::vector<int> >(
        vertices_count, 
        std::vector<int>(vertices_count));

    edges.resize(edges_count);

    int vertex_first, vertex_second, cost;
    for (int indexi = 0; indexi < edges_count; ++indexi) {
        std::cin >> vertex_first >> vertex_second >> cost;
        costs[vertex_first - 1][vertex_second - 1] = cost;
        costs[vertex_second - 1][vertex_first - 1] = cost;
        edges[indexi].first.first = vertex_first - 1;
        edges[indexi].first.second = vertex_second - 1;
        edges[indexi].second = cost;
    }

    dinic();

    std::vector<int> answ;
    int sum = 0;
    sort(best_cut_part_vertices.begin(), best_cut_part_vertices.end());

    for (int edges_i = 0; edges_i < edges_count; ++edges_i) {
        bool first_found = find(
            best_cut_part_vertices.begin(),
            best_cut_part_vertices.end(),
            edges[edges_i].first.first) == best_cut_part_vertices.end();
        bool second_found = find(
            best_cut_part_vertices.begin(),
            best_cut_part_vertices.end(),
            edges[edges_i].first.second) == best_cut_part_vertices.end();

        if (first_found != second_found) {
            sum += edges[edges_i].second;
            answ.push_back(edges_i + 1);
        }
    }

    std::cout << answ.size() << " " << sum << "\n";
    for (int answ_i = 0; answ_i < answ.size(); ++answ_i) {
        std::cout << answ[answ_i] << " ";
    }

    return 0;
}
