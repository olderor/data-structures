// https://contest.yandex.ru/contest/3575/problems/E/

#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#pragma warning(disable:4996) 

struct node {
    int size = 1;
    int value;

    bool is_circle = false;

    bool reversed = false;

    node *left = nullptr;
    node *right = nullptr;
    node *parent = nullptr;

    node() {}

    explicit node(int value) : value(value) {}
};

int size(node *root) {
    return root ? root->size : 0;
}

void update(node *root) {
    if (!root) {
        return;
    }
    root->size = 1 + size(root->left) + size(root->right);
    if (root->left) {
        root->left->parent = root;
    }
    if (root->right) {
        root->right->parent = root;
    }
    root->parent = nullptr;
}

void mark_reverse(node *root) {
    if (!root) {
        return;
    }
    root->reversed = !root->reversed;
}

void do_reverse_if_need(node *root) {
    if (!root) {
        return;
    }
    if (!root->reversed) {
        return;
    }
    std::swap(root->left, root->right);
    root->reversed = false;
    mark_reverse(root->left);
    mark_reverse(root->right);
}

void merge(node *left, node *right, node *&result) {
    do_reverse_if_need(left);
    do_reverse_if_need(right);

    if (!left) {
        result = right;
    } else if (!right) {
        result = left;
    } else if ((rand() % (size(left) + size(right))) < size(left)) {
        merge(left->right, right, left->right);
        result = left;
    } else {
        merge(left, right->left, right->left);
        result = right;
    }
    update(result);
}

void split(node *root, node *&left, node *&right, const int position) {
    do_reverse_if_need(root);

    if (!root) {
        left = right = nullptr;
        return;
    }
    if (position <= size(root->left)) {
        split(root->left, left, root->left, position);
        right = root;
    } else {
        split(root->right, root->right, right, position - 1 - size(root->left));
        left = root;
    }
    update(root);
}

node* get_root(node *current) {
    while (current->parent) {
        current = current->parent;
    }
    return current;
}

node* get_sub_root(node *current) {
    if (!current->parent) {
        return current;
    }
    while (current->parent->parent) {
        current = current->parent;
    }
    return current;
}

std::vector<node*> cities;



void add_way(int first, int second) {

    node *first_sub_root = get_sub_root(cities[first]);
    node *second_sub_root = get_sub_root(cities[second]);

    node *first_root = get_root(first_sub_root);
    node *second_root = get_root(second_sub_root);

    if (first_root == second_root) {
        first_root->is_circle = true;
        return;
    }

    do_reverse_if_need(first_root);
    do_reverse_if_need(second_root);

    if (first_sub_root->parent) {
        if (first_sub_root->parent->left == first_sub_root) {
            mark_reverse(first_root);
        }
    } else if (first_sub_root->right) {
        mark_reverse(first_root);
    }

    if (second_sub_root->parent) {
        if (second_sub_root->parent->right == second_sub_root) {
            mark_reverse(second_root);
        }
    } else if (second_sub_root->left) {
        mark_reverse(second_root);
    }

    node *result;
    merge(first_root, second_root, result);
}

int find_index(node *root) {
    do_reverse_if_need(root);
    int index = size(root->left) + 1;
    while (root->parent) {
        if (root->parent->right == root) {
            index += size(root->parent->left) + 1;
        }
        if (root->parent->reversed) {
            index = size(root->parent) - index + 1;
        }
        root = root->parent;
    }
    return index;
}

int find_way(const int first, const int second) {
    node *root = get_root(cities[first]);
    if (root != get_root(cities[second])) {
        return -1;
    }
    int first_index = find_index(cities[first]);
    int second_index = find_index(cities[second]);
    if (first_index > second_index) {
        std::swap(first_index, second_index);
    }
    int way = second_index - first_index - 1;
    if (!root->is_circle) {
        return way;
    }
    int other_way = first_index + size(root) - second_index - 1;
    if (way < other_way) {
        return way;
    }
    return other_way;
}

void delete_way(const int first, const int second) {
    int first_index = find_index(cities[first]);
    int second_index = find_index(cities[second]);
    if (first_index > second_index) {
        std::swap(first_index, second_index);
    }
    node *root = get_root(cities[first]);
    bool is_circle = root->is_circle;
    root->is_circle = false;
    if (is_circle && first_index == 1 && second_index == size(root)) {
        return;
    }
    node *left, *right;
    split(root, left, right, first_index);
    if (is_circle) {
        node *result;
        merge(right, left, result);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int cities_count, ways_count, queries_count;
    scanf("%d %d %d", &cities_count, &ways_count, &queries_count);

    cities.resize(cities_count);
    for (int i = 0; i < cities_count; ++i) {
        cities[i] = new node(i);
    }

    int first, second;
    char command;
    for (int i = 0; i < ways_count; ++i) {
        scanf("%d %d", &first, &second);
        add_way(first - 1, second - 1);
    }
    for (int index = 0; index < queries_count; ++index) {
        scanf("\n%c %d %d", &command, &first, &second);
        if (command == '+') {
            if (first == second) {
                continue;
            }
            add_way(first - 1, second - 1);
            continue;
        }
        if (command == '?') {
            if (first == second) {
                printf("0\n");
                continue;
            }
            printf("%d\n", find_way(first - 1, second - 1));
            continue;
        }
        if (first == second) {
            continue;
        }
        delete_way(first - 1, second - 1);
    }


    return 0;
}
