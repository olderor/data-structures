// https://contest.yandex.ru/contest/4479/problems/B/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>

struct edge {
    int from;
    int to;
    double cost;
    double flow;
    int index;
};

int vertices_count;
double edges_count;
std::vector<edge*> initial_edges;
std::vector<double> vertices_density;
std::vector< std::vector<edge*> > graph;
std::vector<int> used;
std::vector<int> height;
std::vector<int> heights_count;
std::vector<double> excess;
std::queue<int> vertices_queue;
std::vector<int> answer;

void add_edge(int from, int to, double cost) {
    edge *new_edge = new edge();
    new_edge->from = from;
    new_edge->to = to;
    new_edge->cost = cost;
    new_edge->index = graph[to].size();
    graph[from].push_back(new_edge);

    new_edge = new edge();
    new_edge->from = to;
    new_edge->to = from;
    new_edge->index = graph[from].size() - 1;
    graph[to].push_back(new_edge);
}

void enqueue(int vertex) {
    if (used[vertex]) {
        return;
    }
    if (excess[vertex] <= 0) {
        return;
    }
    used[vertex] = 1;
    vertices_queue.push(vertex);
}

void push(edge *cur_edge) {
    if (height[cur_edge->from] <= height[cur_edge->to]) {
        return;
    }
    double dif = cur_edge->cost - cur_edge->flow;
    if (dif > excess[cur_edge->from]) {
        dif = excess[cur_edge->from];
    }
    if (!dif) {
        return;
    }

    excess[cur_edge->to] += dif;
    excess[cur_edge->from] -= dif;
    cur_edge->flow += dif;
    graph[cur_edge->to][cur_edge->index]->flow -= dif;
    enqueue(cur_edge->to);
}

void remove_height(int min_height) {
    for (int v = 0; v < vertices_count; ++v) {
        if (height[v] >= min_height) {
            --heights_count[height[v]];
            if (height[v] < vertices_count + 1) {
                height[v] = vertices_count + 1;
            }
            ++heights_count[height[v]];
            enqueue(v);
        }
    }
}

void relabel(int vertex) {
    --heights_count[height[vertex]];
    height[vertex] = vertices_count * 2;
    for (int i = 0; i < graph[vertex].size(); ++i) {
        if (graph[vertex][i]->cost - graph[vertex][i]->flow <= 0) {
            continue;
        }
        height[vertex] = std::min(height[vertex], height[graph[vertex][i]->to] + 1);
    }
    ++heights_count[height[vertex]];
    enqueue(vertex);
}

void discharge(int vertex) {
    if (excess[vertex] > 0) {
        for (int i = 0; i < graph[vertex].size(); ++i) {
            push(graph[vertex][i]);
        }
    }
    if (excess[vertex] > 0) {
        if (heights_count[height[vertex]] != 1) {
            relabel(vertex);
            return;
        }
        remove_height(height[vertex]);
    }
}

double maxflow() {
    heights_count[0] = vertices_count - 1;
    heights_count[vertices_count] = 1;
    height[0] = vertices_count;
    used[0] = 1;
    used[vertices_count - 1] = 1;

    for (int i = 0; i < graph[0].size(); ++i) {
        excess[0] += graph[0][i]->cost;
        push(graph[0][i]);
    }

    while (!vertices_queue.empty()) {
        int vertex = vertices_queue.front();
        vertices_queue.pop();
        used[vertex] = 0;
        discharge(vertex);
    }

    double result = 0;
    for (int i = 0; i < graph[0].size(); ++i) {
        result += graph[0][i]->flow;
    }
    return result;
}

void clear(int vs) {
    graph.clear();
    graph.resize(vertices_count);

    height.clear();
    height.resize(vs);
    excess.clear();
    excess.resize(vs);
    used.clear();
    used.resize(vs);
    heights_count.clear();
    heights_count.resize(vs);
    while (!vertices_queue.empty()) {
        vertices_queue.pop();
    }
}

double get_maxflow(double value) {
    clear(vertices_count + vertices_count);
    for (int i = 0; i < initial_edges.size(); ++i) {
        add_edge(initial_edges[i]->from, initial_edges[i]->to, 1);
        add_edge(initial_edges[i]->to, initial_edges[i]->from, 1);
    }
    for (int i = 1; i < vertices_count - 1; ++i) {
        add_edge(0, i, edges_count);
        add_edge(i, vertices_count - 1, edges_count + 2 * value - vertices_density[i]);
    }
    return maxflow();
}

int main() {

    std::cin >> vertices_count >> edges_count;

    if (edges_count == 0) {
        std::cout << "1\n1\n";
        return 0;
    }

    vertices_count += 2;

    initial_edges.resize(edges_count);
    vertices_density.resize(vertices_count);

    int from, to;
    for (int i = 0; i < edges_count; ++i) {
        std::cin >> from >> to;
        initial_edges[i] = new edge();
        initial_edges[i]->from = from;
        initial_edges[i]->to = to;
        ++vertices_density[from];
        ++vertices_density[to];
    }

    double left = 0;
    double right = edges_count;
    while (right - left >= 1.0 / ((vertices_count - 2) * (vertices_count - 2))) {
        double mid = (left + right) / 2;
        double flow = get_maxflow(mid);
        if (((vertices_count - 2) * edges_count - flow) / 2.0 > 0) {
            left = mid;
        } else {
            right = mid;
        }
    }

    get_maxflow(left);

    for (int i = 1; i < vertices_count - 1; ++i) {
        if (height[i] >= height[0]) {
            answer.push_back(i);
        }
    }
    std::cout << answer.size() << '\n';
    for (int i = 0; i < answer.size(); ++i) {
        std::cout << answer[i] << '\n';
    }

    return 0;
}
