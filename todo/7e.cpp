// https://contest.yandex.ru/contest/4479/problems/E/
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#pragma warning(disable:4996)

int rint() {
    int result = 0;
    char ch;
    ch = getchar();
    while (ch < '0' || ch > '9') {
        ch = getchar();
    }
    result = ch - '0';
    ch = getchar();
    while (ch >= '0' && ch <= '9') {
        result = result * 10 + (ch - '0');
        ch = getchar();
    }
    return result;
}

struct edge {
    int from;
    int to;
    int cost;
    int flow;
    int index;
};

int vertices_count, edges_count;

std::vector< std::vector<edge*> > graph;
std::vector< std::vector<edge*> > edges;
std::vector<int> used;
std::vector<int> height;
std::vector<int> heights_count;
std::vector<long long> excess;
std::queue<int> vertices_queue;

void add_edge(int from, int to, int cost) {
    edge *new_edge = new edge();
    new_edge->from = from;
    new_edge->to = to;
    new_edge->cost = cost;
    new_edge->index = graph[to].size();
    graph[from].push_back(new_edge);
    edges[from][to] = new_edge;

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
    if (excess[vertex] < 1) {
        return;
    }
    used[vertex] = 1;
    vertices_queue.push(vertex);
}

void push(edge *cur_edge) {
    if (height[cur_edge->from] <= height[cur_edge->to]) {
        return;
    }
    int dif = cur_edge->cost - cur_edge->flow;
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

long long maxflow() {
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

    long long result = 0;
    for (int i = 0; i < graph[0].size(); ++i) {
        result += graph[0][i]->flow;
    }
    return result;
}

void clear(int vs) {
    for (int i = 0; i < graph.size(); ++i) {
        for (int j = 0; j < graph[i].size(); ++j) {
            graph[i][j]->flow = 0;
        }
    }
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


int main() {
    vertices_count = rint();
    edges_count = rint();

    graph.clear();
    graph.resize(vertices_count);
    edges.clear();
    edges.resize(vertices_count);
    for (int i = 0; i < vertices_count; ++i) {
        edges[i].resize(vertices_count);
    }
    clear(vertices_count + 100);

    int from, to;
    long long cost;
    for (int i = 0; i < edges_count; ++i) {
        from = rint();
        to = rint();
        cost = rint();
        add_edge(from - 1, to - 1, cost);
    }

    printf("%lld\n", maxflow());

    int tests = rint();
    int type;
    for (int i = 0; i < tests; ++i) {
        clear(vertices_count + 100);
        type = rint();
        from = rint();
        to = rint();
        if (type == 1) {
            ++edges[from - 1][to - 1]->cost;
        } else {
            --edges[from - 1][to - 1]->cost;
        }
        printf("%lld\n", maxflow());
    }

    return 0;
}
