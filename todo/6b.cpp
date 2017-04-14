// https://contest.yandex.ru/contest/4277/problems/B/

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

int vertices_count;
std::vector<edge*> graph[5004];
long long excess[5004];
int height[5004];
int used[5004];
int heights_count[5004];
std::queue<int> vertices_queue;

void add_edge(int from, int to, int cost) {
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

int m_height, m_width;
int matrix[51][51];

int main() {
    std::cin >> m_height >> m_width;
    vertices_count = m_height * m_width + 2;
    char c;
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            std::cin >> c;
            matrix[i][j] = c - '0';
        }
    }

    long long result = 0;
    for (int height_i = 0; height_i < m_height; ++height_i) {
        for (int width_i = 0; width_i < m_width; ++width_i) {
            result += matrix[height_i][width_i];
            if (!matrix[height_i][width_i]) {
                continue;
            }
            if (height_i % 2 != width_i % 2) {
                add_edge(
                    height_i * m_width + width_i + 1,
                    vertices_count - 1,
                    matrix[height_i][width_i]);
                continue;
            }
            add_edge(
                0,
                height_i * m_width + width_i + 1,
                matrix[height_i][width_i]);

            if (height_i != 0) {
                add_edge(
                    height_i * m_width + width_i + 1,
                    (height_i - 1) * m_width + width_i + 1,
                    1);
            }
            if (width_i != 0) {
                add_edge(
                    height_i * m_width + width_i + 1,
                    height_i * m_width + width_i,
                    1);
            }
            if (height_i + 1 != m_height) {
                add_edge(
                    height_i * m_width + width_i + 1,
                    (height_i + 1) * m_width + width_i + 1,
                    1);
            }
            if (width_i + 1 != m_width) {
                add_edge(
                    height_i * m_width + width_i + 1,
                    height_i * m_width + width_i + 2,
                    1);
            }
        }
    }

    if (result % 2 != 0) {
        std::cout << "Invalid";
        return 0;
    }
    result /= 2;

    long long flow = maxflow();

    if (flow == result && flow != 0) {
        std::cout << "Valid";
    } else {
        std::cout << "Invalid";
    }

    return 0;
}
