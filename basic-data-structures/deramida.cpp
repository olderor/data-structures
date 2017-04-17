// https://contest.yandex.ru/contest/3476/problems/A/

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <bitset>

const int MOD = 1e9 + 7;
const int MAX = 1e5 + 1;

int queries_count, query;

struct deramida {
    int size = 1;
    int value;
    int sum;
    deramida *left = nullptr;
    deramida *right = nullptr;
    explicit deramida(int value) : value(value), sum(value) { }
};

int get_max_size() {
    return 1 + queries_count - query;
}

int get_size(deramida *root) {
    if (!root) {
        return 0;
    }
    return root->size;
}

int get_sum(deramida *root) {
    if (!root) {
        return 0;
    }
    return root->sum;
}

int get_value(deramida *root) {
    if (!root) {
        return 0;
    }
    return root->value;
}


deramida* get_copy(deramida *root) {
    if (!root) {
        return nullptr;
    }
    deramida *res = new deramida(root->value);
    res->size = root->size;
    res->sum = root->sum;
    res->left = root->left;
    res->right = root->right;
    return res;
}

void update(deramida *res) {
    if (!res) {
        return;
    }

    res->size = 1;
    res->size += get_size(res->left);
    res->size += get_size(res->right);
}

deramida* merge_deramidas(deramida *first, deramida *second) {
    deramida *res, *new_res;
    if (!first) {
        res = get_copy(second);
        return res;
    }
    if (!second) {
        res = get_copy(first);
        return res;
    }
    int sum = (get_sum(first) + get_sum(second)) % MOD;
    if ((rand() % (get_size(first) + get_size(second))) > get_size(second)) {
        res = get_copy(first);
        res->right = merge_deramidas(first->right, second);
    } else {
        res = get_copy(second);
        res->left = merge_deramidas(first, second->left);
    }
    res->sum = sum;
    update(res);
    return res;
}

void split(deramida *root, deramida *&left, deramida *&right, const int position) {
    if (!root) {
        left = right = nullptr;
        return;
    }
    deramida *current = get_copy(root);
    if (position <= get_size(current->left)) {
        split(current->left, left, current->left, position);
        right = current;
    } else {
        split(current->right, current->right, right, position - 1 - get_size(current->left));
        left = current;
    }
    update(current);
}

deramida* merge(deramida *first, deramida *second) {

    int first_sum = get_sum(first);
    int second_sum = get_sum(second);

    first = first->left;
    second = second->left;

    if (get_size(first) > get_max_size() && get_size(second) > get_max_size()) {
        deramida *left, *right, *temp;
        split(first, left, temp, get_max_size());
        split(second, temp, right, get_max_size());
        first = left;
        second = right;
    } else if (get_size(first) > get_max_size()) {
        deramida *left, *right, *temp;
        split(first, left, temp, get_max_size());
        split(first, temp, right, get_size(first) - get_max_size());
        first = merge_deramidas(left, right);
    } else if (get_size(second) > get_max_size()) {
        deramida *left, *right, *temp;
        split(second, left, temp, get_max_size());
        split(second, temp, right, get_size(second) - get_max_size());
        second = merge_deramidas(left, right);
    }

    deramida *res = merge_deramidas(first, second);
    deramida *res_root = new deramida(0);
    res_root->sum = (first_sum + second_sum) % MOD;
    res_root->left = res;

    return res_root;
}

deramida *temp = nullptr;

deramida* get_head(deramida *root) {
    if (!root->left) {
        temp = get_copy(root);
        temp->right = nullptr;
        temp->left = new deramida(get_value(temp));
        temp->sum = get_value(temp);
        update(temp);
        return root->right;
    }
    deramida *res = get_copy(root);
    res->left = get_head(res->left);
    update(res);
    return res;
}

deramida* head(deramida *root) {
    deramida *head = get_head(root->left);
    deramida *head_root = new deramida(0);
    head_root->sum = (root->sum - get_sum(temp) + MOD) % MOD;
    head_root->left = head;
    return head_root;
}

deramida* get_tail(deramida *root) {
    if (!root->right) {
        temp = get_copy(root);
        temp->left = new deramida(get_value(temp));
        temp->sum = get_value(temp);
        update(temp);
        return root->left;
    }
    deramida *res = get_copy(root);
    res->right = get_tail(res->right);
    update(res);
    return res;
}

deramida* tail(deramida *root) {
    deramida *tail = get_tail(root->left);
    deramida *tail_root = new deramida(0);
    tail_root->sum = (root->sum - get_sum(temp) + MOD) % MOD;
    tail_root->left = tail;
    return tail_root;
}


int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);


    std::vector<deramida*> versions;

    int elements_count;
    std::cin >> elements_count;
    for (int i = 0; i < elements_count; ++i) {
        int value;
        std::cin >> value;
        deramida *root = new deramida(value);
        root->left = new deramida(value);
        versions.push_back(root);
    }

    std::cin >> queries_count;
    for (query = 0; query < queries_count; ++query) {
        std::string command;
        int version;
        std::cin >> command >> version;
        deramida *res;
        if (command[0] == 'm') {
            int with_version;
            std::cin >> with_version;
            res = merge(versions[version - 1], versions[with_version - 1]);
            versions.push_back(res);
            std::cout << get_sum(res) << '\n';
            continue;
        }
        if (command[0] == 'h') {
            res = head(versions[version - 1]);
            std::cout << get_sum(temp) << '\n';
            std::cout << get_sum(res) << '\n';
            versions.push_back(temp);
            versions.push_back(res);
            continue;
        }
        res = tail(versions[version - 1]);
        std::cout << get_sum(res) << '\n';
        std::cout << get_sum(temp) << '\n';
        versions.push_back(res);
        versions.push_back(temp);
    }

    return 0;
}
