#include <iostream>
#include <vector>
#include <unordered_map>

#pragma warning(disable: 4996)

int _gcd(int first, int second) {
    return second ? _gcd(second, first % second) : first;
}

struct triple {
    int part1_first, part1_second;
    int part2_first, part2_second;
};

struct node {
    int value, index;
    node() {}
    node(int val, int id) : value(val), index(id) {}
};

struct connection {
    int value = -1;
    int index = -1;
};

std::unordered_map<int, std::unordered_map<int, int> > part1_ms;
std::unordered_map<int, std::unordered_map<int, int> > part2_ms;

std::vector<bool> used;
std::vector< std::vector<node> > graph;

std::vector<connection> connections;

bool dfs(int vertex, int parent) {
    used[vertex] = true;

    int to;
    for (int to_index = 0; to_index < graph[vertex].size(); ++to_index) {
        to = graph[vertex][to_index].value;
        if (to == parent) {
            continue;
        }
        if (connections[to].value == -1 ||
            !used[connections[to].value] && dfs(connections[to].value, to)) {
            connections[to].value = vertex;
            connections[to].index = graph[vertex][to_index].index;
            return true;
        }
    }

    return false;
}


int main() {

    int tests_count;
    scanf("%d", &tests_count);
    int values_count;
    std::vector<triple> values;
    int gcd;
    int part1_size, part2_size;
    std::vector<int> answer;
    for (int test = 0; test < tests_count; ++test) {
        scanf("%d", &values_count);
        part1_size = 0, part2_size = 0;
        values.resize(values_count);
        answer.clear();
        graph.clear();
        graph.resize(values_count);
        part1_ms.clear();
        part2_ms.clear();
        connections.clear();
        connections.resize(values_count);

        for (int index = 0; index < values_count; ++index) {
            scanf(
                "%d %d %d", 
                &values[index].part1_first, 
                &values[index].part1_second, 
                &values[index].part2_second);

            if (values[index].part1_second == 0 && values[index].part2_second == 0) {
                continue;
            }

            values[index].part2_first = values[index].part1_second;

            if (values[index].part1_first == 0) {
                values[index].part1_second = 1;
            }
            if (values[index].part2_first == 0) {
                values[index].part2_second = 1;
            }

            if (values[index].part1_first < 0) {
                values[index].part1_first = -values[index].part1_first;
                values[index].part1_second = -values[index].part1_second;
            }

            if (values[index].part2_first < 0) {
                values[index].part2_first = -values[index].part2_first;
                values[index].part2_second = -values[index].part2_second;
            }

            gcd = _gcd(values[index].part1_first, abs(values[index].part1_second));
            if (gcd != 0) {
                values[index].part1_first /= gcd;
                values[index].part1_second /= gcd;
            }
            gcd = _gcd(values[index].part2_first, abs(values[index].part2_second));
            if (gcd != 0) {
                values[index].part2_first /= gcd;
                values[index].part2_second /= gcd;
            }

            int part1_val = part1_ms[values[index].part1_first][values[index].part1_second];
            int part2_val = part2_ms[values[index].part2_first][values[index].part2_second];

            if (part1_val == 0) {
                part1_ms[values[index].part1_first][values[index].part1_second] = ++part1_size;
                part1_val = part1_size;
            }
            if (part2_val == 0) {
                part2_ms[values[index].part2_first][values[index].part2_second] = ++part2_size;
                part2_val = part2_size;
            }

            graph[part1_val - 1].push_back(node(part2_val - 1, index));
        }

        for (int part1_index = 0; part1_index < part1_size; ++part1_index) {
            used.clear();
            used.resize(part1_size);
            dfs(part1_index, -1);
        }
        for (int part2_index = 0; part2_index < part2_size; ++part2_index) {
            if (connections[part2_index].value != -1) {
                answer.push_back(connections[part2_index].index);
            }
        }
        if (answer.size() == 0) {
            printf("1\n1\n");
            continue;
        }
        printf("%d\n", answer.size());
        for (int answer_index = 0; answer_index < answer.size(); ++answer_index) {
            printf("%d ", answer[answer_index] + 1);
        }
        printf("\n");
    }

    return 0;
}
