#include <algorithm>
#include <iostream>
#include <vector>

#pragma warning(disable:4996)

int tree[129][129][129];
int size;

void update(int coord_x, int coord_y, int coord_z, int dif) {
    for (int i = coord_x; i < size; i = i | (i + 1)) {
        for (int j = coord_y; j < size; j = j | (j + 1)) {
            for (int k = coord_z; k < size; k = k | (k + 1)) {
                tree[i][j][k] += dif;
            }
        }
    }
}

int get(int coord_x, int coord_y, int coord_z) {
    int res = 0;
    for (int i = coord_x; i >= 0; i -= (i + 1) & -(i + 1)) {
        for (int j = coord_y; j >= 0; j -= (j + 1) & -(j + 1)) {
            for (int k = coord_z; k >= 0; k -= (k + 1) & -(k + 1)) {
                res += tree[i][j][k];
            }
        }
    }
    return res;
}

int main() {
    int queries_count;
    scanf("%d %d", &size, &queries_count);
    for (int query = 0; query < queries_count; ++query) {
        int command;
        scanf("%d", &command);
        if (command == 1) {
            int coord_x, coord_y, coord_z, dif;
            scanf("%d %d %d %d", &coord_x, &coord_y, &coord_z, &dif);
            update(
                coord_x,
                coord_y,
                coord_z,
                dif
            );
            continue;
        }
        int coord_x_min, coord_y_min, coord_z_min;
        int coord_x_max, coord_y_max, coord_z_max;
        scanf("%d %d %d", &coord_x_min, &coord_y_min, &coord_z_min);
        scanf("%d %d %d", &coord_x_max, &coord_y_max, &coord_z_max);
        int res = get(
            coord_x_max,
            coord_y_max,
            coord_z_max
        ) + get(
            coord_x_min - 1,
            coord_y_min - 1,
            coord_z_max
        ) + get(
            coord_x_min - 1,
            coord_y_max,
            coord_z_min - 1
        ) + get(
            coord_x_max,
            coord_y_min - 1,
            coord_z_min - 1
        ) - get(
            coord_x_min - 1,
            coord_y_max,
            coord_z_max
        ) - get(
            coord_x_max,
            coord_y_min - 1,
            coord_z_max
        ) - get(
            coord_x_max,
            coord_y_max,
            coord_z_min - 1
        ) - get(
            coord_x_min - 1,
            coord_y_min - 1,
            coord_z_min - 1
        );
        printf("%d\n", res);
    }

    return 0;
}
