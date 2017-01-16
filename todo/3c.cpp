// https://contest.yandex.ru/contest/3476/problems/C/

#include <iostream>
#include <istream>
#include <vector>

#pragma warning(disable:4996) 


struct space {
    int start, end;
    space(const int start, const int end) : start(start), end(end) {}
};

std::vector<std::vector<space*>> spaces;


int find(std::vector<space*> &elements, const int end) {
    int left = 0;
    int right = elements.size() - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (end <= elements[mid]->end) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

struct node {
    std::vector<space*> value;
    node *left = nullptr;
    node *right = nullptr;
};

std::vector<space*> merge(
    std::vector<space*> &first, 
    std::vector<space*> &second
) {
    std::vector<space*> result;

    int left = 0;
    int right = 0;

    while (left < first.size() && right < second.size()) {
        if (first[left]->start <= second[right]->start) {
            if (first[left]->end >= second[right]->end) {
                ++right;
            } else {
                result.push_back(first[left++]);
            }
        } else {
            if (first[left]->end <= second[right]->end) {
                ++left;
            } else {
                result.push_back(second[right++]);
            }
        }
    }
    while (left < first.size()) {
        result.push_back(first[left++]);
    }
    while (right < second.size()) {
        result.push_back(second[right++]);
    }

    return result;
}

node* build(const int left, const int right) {
    node *root = new node();
    if (left == right) {
        root->value = spaces[left];
    } else {
        const int mid = (left + right) / 2;
        root->left = build(left, mid);
        root->right = build(mid + 1, right);
        root->value = merge(root->left->value, root->right->value);
    }
    return root;
}

bool check(node *root, const int start, const int end) {
    const int index = find(root->value, end);
    space *space = root->value[index];
    return space->start <= start && end <= space->end;
}

int get(node *root, const int left, const int right, const int start, const int end) {
    
    if (root->left == nullptr) {
        return left;
    }

    const int mid = (left + right) / 2;
    if (check(root->left, start, end)) {
        return get(root->left, left, mid, start, end);
    } else {
        return get(root->right, mid + 1, right, start, end);
    }

    return left;
}

void insert_space(
    const int left,
    const int right,
    const int index
) {
    int space_index = find(spaces[index], right);
    int end = spaces[index][space_index]->end;
    spaces[index][space_index]->end = left;
    spaces[index].insert(
        spaces[index].begin() + space_index + 1,
        new space(right, end)
    );
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int height, width, segments_count;
    scanf("%d %d %d", &height, &width, &segments_count);

    spaces.resize(width);
    for (int i = 0; i < width; ++i) {
        spaces[i].push_back(new space(0, height - 1));
    }


    int y_pos, x1_pos, x2_pos;
    for (int i = 0; i < segments_count; ++i) {
        scanf("%d %d %d", &y_pos, &x1_pos, &x2_pos);
        insert_space(x1_pos - 1, x2_pos - 1, y_pos - 1);
    }

    node *root = build(0, width - 1);

    int queries_count;
    scanf("%d", &queries_count);

    int parameter = 0, x_q, y_q;
    for (int query = 0; query < queries_count; ++query) {
        scanf("%d %d", &x_q, &y_q);

        const int start = x_q - 1 + parameter,
            end = y_q - 1 + parameter;

        if (start > end) {
            if (check(root, end, start)) {
                parameter = get(root, 0, width - 1, end, start) + 1;
            } else {
                parameter = 0;
            }
        } else {
            if (check(root, start, end)) {
                parameter = get(root, 0, width - 1, start, end) + 1;
            } else {
                parameter = 0;
            }
        }

        printf("%d\n", parameter);
    }


    return 0;
}
