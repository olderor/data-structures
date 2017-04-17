#include <algorithm>
#include <iostream>
#include <vector>

#pragma warning(disable:4996)

int tree[4001][4001];
int values[1001][1001];
int width, height;

void build_column(
    int current, 
    int left, 
    int right,
    int current_column,
    int current_left,
    int current_right
) {
    if (current_left == current_right) {
        if (left == right) {
            tree[current][current_column] = values[left][current_left];
        } else {
            tree[current][current_column] = std::max(
                tree[current * 2][current_column], 
                tree[current * 2 + 1][current_column]
            );
        }
    } else {
        int mid = (current_left + current_right) / 2;
        build_column(current, left, right, current_column * 2, current_left, mid);
        build_column(current, left, right, current_column * 2 + 1, mid + 1, current_right);
        tree[current][current_column] = std::max(
            tree[current][current_column * 2], 
            tree[current][current_column * 2 + 1]
        );
    }
}

void build_row(int current, int left, int right) {
    if (left != right) {
        int mid = (left + right) / 2;
        build_row(current * 2, left, mid);
        build_row(current * 2 + 1, mid + 1, right);
    }
    build_column(current, left, right, 1, 0, width - 1);
}

int get_column(
    int current,
    int current_column,
    int left, 
    int right, 
    int left_column, 
    int right_column
) {
    if (left_column > right_column) {
        return -10000;
    }
    if (left_column == left && right == right_column) {
        return tree[current][current_column];
    }
    int mid = (left + right) / 2;
    return std::max(
        get_column(
            current, 
            current_column * 2, 
            left,
            mid, 
            left_column,
            std::min(right_column, mid)
        ),
        get_column(
            current, 
            current_column * 2 + 1,
            mid + 1, 
            right, 
            std::max(left_column, mid + 1),
            right_column
        )
    );
}

int get_row(
    int current,
    int left, 
    int right, 
    int left_row, 
    int right_row, 
    int left_column, 
    int right_column
) {
    if (left_row > right_row) {
        return -10000;
    }
    if (left_row == left && right == right_row) {
        return get_column(current, 1, 0, width - 1, left_column, right_column);
    }
    int mid = (left + right) / 2;
    return std::max(
        get_row(current * 2, 
            left, 
            mid, 
            left_row, 
            std::min(right_row, mid), 
            left_column, 
            right_column
        ),
        get_row(
            current * 2 + 1, 
            mid + 1, right, 
            std::max(left_row, mid + 1), 
            right_row, 
            left_column,
            right_column
        )
    );
}

void update_column(
    int current, 
    int left_row,
    int right_row, 
    int current_column, 
    int left_column,
    int right_column, 
    int row,
    int column,
    int new_val
) {
    if (left_column == right_column) {
        if (left_row == right_row) {
            tree[current][current_column] = new_val;
        } else {
            tree[current][current_column] = std::max(
                tree[current * 2][current_column], 
                tree[current * 2 + 1][current_column]
            );
        }
    } else {
        int mid = (left_column + right_column) / 2;
        if (column <= mid) {
            update_column(
                current,
                left_row,
                right_row,
                current_column * 2,
                left_column,
                mid,
                row,
                column,
                new_val
            );
        } else {
            update_column(
                current, 
                left_row,
                right_row, 
                current_column * 2 + 1,
                mid + 1, 
                right_column,
                row,
                column,
                new_val
            );
        }
        tree[current][current_column] = std::max(
            tree[current][current_column * 2], 
            tree[current][current_column * 2 + 1]
        );
    }
}

void update_row(
    int vrow,
    int lrow, 
    int rrow, 
    int row, 
    int column,
    int new_val
) {
    if (lrow != rrow) {
        int mrow = (lrow + rrow) / 2;
        if (row <= mrow) {
            update_row(vrow * 2, lrow, mrow, row, column, new_val);
        } else {
            update_row(vrow * 2 + 1, mrow + 1, rrow, row, column, new_val);
        }
    }
    update_column(vrow, lrow, rrow, 1, 0, width - 1, row, column, new_val);
}


int main() {
    int queries_count;
    scanf("%d%d%d", &height, &width, &queries_count);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            scanf("%d", &values[i][j]);
        }
    }
    build_row(1, 0, height - 1);

    for (int query = 0; query < queries_count; ++query) {
        char command;
        scanf("%c", &command);
        scanf("%c", &command);
        if (command == '?') {
            int from_row, to_row, from_col, to_col;
            scanf("%d%d%d%d", &from_row, &to_row, &from_col, &to_col);
            printf("%d\n", 
                get_row(
                    1, 
                    0, 
                    height - 1, 
                    from_row - 1,
                    to_row - 1, 
                    from_col - 1, 
                    to_col - 1
                ));
            continue;
        }
        int row, col, value;
        scanf("%d%d%d", &row, &col, &value);
        update_row(1, 0, height - 1, row - 1, col - 1, value);
    }

    return 0;
}
