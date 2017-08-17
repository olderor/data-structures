#include <iostream>
#include <vector>
#include <queue>

#pragma warning(disable:4996)

struct edge {
    int from;
    int to;
    int cost;
    int flow;
    int index;
};

int vertices_count, edges_count;
std::vector<edge*> edges;
std::vector<edge*> graph[1555];
long long excess[1555];
int height[1555];
int used[1555];
int heights_count[1555];
std::queue<int> vertices_queue;

void add_edge(int from, int to, int cost) {
    edge *new_edge = new edge();
    new_edge->from = from;
    new_edge->to = to;
    new_edge->cost = cost;
    new_edge->index = graph[to].size();
    graph[from].push_back(new_edge);
    edges.push_back(new_edge);

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
    if (dif == 0) {
        return;
    }

    cur_edge->flow += dif;
    graph[cur_edge->to][cur_edge->index]->flow -= dif;
    excess[cur_edge->to] += dif;
    excess[cur_edge->from] -= dif;

    enqueue(cur_edge->to);
}

void remove_height(int min_heigth) {
    for (int v = 0; v < vertices_count; ++v) {
        if (height[v] < min_heigth) {
            continue;
        }
        --heights_count[height[v]];
        if (height[v] < vertices_count + 1) {
            height[v] = vertices_count + 1;
        }
        ++heights_count[height[v]];

        enqueue(v);
    }
}

void relabel(int vertex) {
    --heights_count[height[vertex]];
    height[vertex] = vertices_count * 2;

    for (int i = 0; i < graph[vertex].size(); ++i) {
        if (graph[vertex][i]->cost - graph[vertex][i]->flow > 0) {
            if (height[vertex] > height[graph[vertex][i]->to] + 1) {
                height[vertex] = height[graph[vertex][i]->to] + 1;
            }
        }
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

int main() {
    scanf("%d %d", &vertices_count, &edges_count);

    int from, to;
    long long cost;
    for (int i = 0; i < edges_count; ++i) {
        scanf("%d %d %d", &from, &to, &cost);
        add_edge(from - 1, to - 1, cost);
    }

    printf("%lld\n", maxflow());

    for (int i = 0; i < edges_count; ++i) {
        printf("%d\n", edges[i]->flow);
    }
    
    /*
    min cut
    for (int i = 1; i < vertices_count - 1; ++i) {
        if (height[i] >= height[0]) {
            answer.push_back(i);
        }
    }
    std::cout << answer.size() << '\n';
    for (int i = 0; i < answer.size(); ++i) {
        std::cout << answer[i] << '\n';
    }
    */

    return 0;
}
