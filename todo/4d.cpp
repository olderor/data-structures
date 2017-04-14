// https://contest.yandex.ru/contest/3573/run-report/3634426/

#include <iostream> 
#include <istream> 
#include <fstream> 
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <utility>

struct node {
    int size, value, priority;
    long long sum;
    node *left = nullptr;
    node *right = nullptr;
    node() {}
    explicit node(int value) : 
        value(value), 
        sum(value), 
        priority((rand() << 16u) + unsigned(rand())) 
    {}
};

int size(node *root) {
    return root ? root->size : 0;
}

long long sum(node *root) {
    return root ? root->sum : 0;
}

void update(node *root) {
    if (!root) {
        return;
    }
    root->sum = root->value + sum(root->left) + sum(root->right);
    root->size = 1 + size(root->left) + size(root->right);
}

void merge(node *left, node *right, node *&result) {
    if (!left) {
        result = right;
    } else if (!right) {
        result = left;
    } else if (left->priority > right->priority) {
        merge(left->right, right, left->right);
        result = left;
    } else {
        merge(left, right->left, right->left);
        result = right;
    }
    update(result);
}

void split(node *root, node *&left, node *&right, int position) {
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

void insert(node *&root, int item) {
    merge(root, new node(item), root);
}

void print(node *root) {
    if (!root) {
        return;
    }

    print(root->left);
    std::cout << root->value << " ";
    print(root->right);
}

long long sum(node *&odds, node *&evens, int left, int right) {
    if (left > right) {
        return 0;
    }

    int odds_left = (left + 1) / 2;
    int odds_right = right / 2;
    int evens_left = left / 2;
    int evens_right = (right + 1) / 2 - 1;

    node *odds_before_left, *odds_after_left;
    split(
        odds, 
        odds_before_left,
        odds_after_left,
        odds_left
    );

    node *odds_between_left_and_right, *odds_after_right;
    split(
        odds_after_left,
        odds_between_left_and_right, 
        odds_after_right, 
        odds_right - odds_left + 1
    );

    node *evens_before_left, *evens_after_left;
    split(
        evens,
        evens_before_left,
        evens_after_left, 
        evens_left
    );

    node *evens_between_left_and_right, *evens_after_right;
    split(
        evens_after_left, 
        evens_between_left_and_right,
        evens_after_right, 
        evens_right - evens_left + 1
    );

    long long res = sum(odds_between_left_and_right) + sum(evens_between_left_and_right);

    node *new_odds;
    merge(odds_before_left, odds_between_left_and_right, new_odds);
    merge(new_odds, odds_after_right, odds);

    node *new_evens;
    merge(evens_before_left, evens_between_left_and_right, new_evens);
    merge(new_evens, evens_after_right, evens);

    return res;
}

void reorder(node *&odds, node *&evens, int left, int right) {
    if (left > right) {
        return;
    }

    int odds_left = (left + 1) / 2;
    int odds_right = right / 2;
    int evens_left = left / 2;
    int evens_right = (right + 1) / 2 - 1;

    node *odds_before_left, *odds_after_left;
    split(
        odds, 
        odds_before_left, 
        odds_after_left,
        odds_left
    );

    node *odds_between_left_and_right, *odds_after_right;
    split(
        odds_after_left, 
        odds_between_left_and_right,
        odds_after_right, 
        odds_right - odds_left + 1
    );

    node *evens_before_left, *evens_after_left;
    split(
        evens, 
        evens_before_left,
        evens_after_left, 
        evens_left
    );

    node *evens_between_left_and_right, *evens_after_right;
    split(
        evens_after_left, 
        evens_between_left_and_right, 
        evens_after_right,
        evens_right - evens_left + 1
    );


    node *new_odds;
    merge(odds_before_left, evens_between_left_and_right, new_odds);
    merge(new_odds, odds_after_right, odds);

    node *new_evens;
    merge(evens_before_left, odds_between_left_and_right, new_evens);
    merge(new_evens, evens_after_right, evens);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int size, queries;
    std::cin >> size >> queries;

    int index = 1;
    while (!(size == 0 && queries == 0)) {
        std::cout << "Test " << index++ << ":\n";

        int element;
        node *odds = nullptr;
        node *evens = nullptr;
        for (int i = 1; i <= size; ++i) {
            std::cin >> element;
            insert(i % 2 ? odds : evens, element);
        }

        for (int i = 0; i < queries; ++i) {
            int command, left, right;
            std::cin >> command >> left >> right;
            if (command == 1) {
                reorder(odds, evens, left - 1, right - 1);
            } else {
                std::cout << sum(odds, evens, left - 1, right - 1) << "\n";
            }
        }
        std::cout << "\n";

        std::cin >> size >> queries;
    }

    return 0;
}
