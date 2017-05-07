// https://contest.yandex.ru/contest/4479/problems/D/

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <queue>
#include <utility>

#pragma warning(disable:4996)

std::vector<int> rows;
std::vector<int> columns;
std::vector<int> matchings;
std::vector<int> parents;
std::vector<int> min_values;
std::vector<int> used;
std::vector< std::vector<int> > values;
std::vector<int> answer;

const int INF = 2147483647;

int matrix_size;

int find_free_column(int free_column) {
    used[free_column] = 1;
    int free_row = matchings[free_column];
    int delta = INF;
    int current_column;
    for (int column = 1; column <= matrix_size; ++column) {
        if (used[column]) {
            continue;
        }
        int current_min_value = values[free_row - 1][column - 1] - rows[free_row] - columns[column];
        if (current_min_value < min_values[column]) {
            min_values[column] = current_min_value;
            parents[column] = free_column;
        }
        if (min_values[column] < delta) {
            delta = min_values[column];
            current_column = column;
        }
    }
    for (int column = 0; column <= matrix_size; ++column) {
        if (!used[column]) {
            min_values[column] -= delta;
            continue;
        }
        rows[matchings[column]] += delta;
        columns[column] -= delta;
    }
    return current_column;
}

int restore(int free_column) {
    int current_column = parents[free_column];
    matchings[free_column] = matchings[current_column];
    return current_column;
}

int main() {
    scanf("%d", &matrix_size);

    values.resize(matrix_size);
    rows.resize(matrix_size + 1);
    columns.resize(matrix_size + 1);
    matchings.resize(matrix_size + 1);
    parents.resize(matrix_size + 1);

    for (int i = 0; i < matrix_size; ++i) {
        values[i].resize(matrix_size);
        for (int j = 0; j < matrix_size; ++j) {
            scanf("%d", &values[i][j]);
        }
    }

    int free_column;
    for (int row = 1; row <= matrix_size; ++row) {
        matchings[0] = row;
        min_values.clear();
        min_values.resize(matrix_size + 1, INF);
        used.clear();
        used.resize(matrix_size + 1);

        free_column = find_free_column(0);
        while (matchings[free_column] != 0) {
            free_column = find_free_column(free_column);
        }
        free_column = restore(free_column);
        while (free_column) {
            free_column = restore(free_column);
        }
    }

    printf("%d\n", -columns[0]);

    answer.resize(matrix_size + 1);
    for (int j = 1; j <= matrix_size; ++j) {
        answer[matchings[j]] = j;
    }

    for (int j = 1; j <= matrix_size; ++j) {
        printf("%d ", answer[j]);
    }

    return 0;
}
