// https://contest.yandex.ru/contest/3575/problems/G/

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#pragma warning(disable:4996) 


struct hash_element {
    int key;
    int value;

    hash_element(int key, int value) : key(key), value(value) {}

    hash_element* get_copy() {
        return new hash_element(key, value);
    }
};

const int table_size = 202;

class hash_map {
private:
    hash_element **table;

public:
    hash_map() {
        table = new hash_element*[table_size];
        for (int i = 0; i < table_size; ++i) {
            table[i] = nullptr;
        }
    }

    int get(int key) {
        int hash = (key % table_size);
        while (table[hash] && table[hash]->key != key) {
            hash = (hash + 1) % table_size;
        }
        if (!table[hash]) {
            return -1;
        } else {
            return table[hash]->value;
        }
    }

    void update(int key, int value) {
        int hash = (key % table_size);
        while (table[hash] && table[hash]->key != key) {
            hash = (hash + 1) % table_size;
        }
        if (!table[hash]) {
            return;
        }
        table[hash]->value = value;
    }

    void increase(int key) {
        int hash = (key % table_size);
        while (table[hash] && table[hash]->key != key) {
            hash = (hash + 1) % table_size;
        }
        if (!table[hash]) {
            return;
        }
        ++table[hash]->value;
    }

    void put(int key, int value) {
        int hash = (key % table_size);
        while (table[hash] && table[hash]->key != key) {
            hash = (hash + 1) % table_size;
        }
        table[hash] = new hash_element(key, value);
    }

    void clear() {
        for (int i = 0; i < table_size; ++i) {
            if (table[i]) {
                table[i] = nullptr;
            }
        }
    }

    void copy(hash_map other) {
        clear();
        for (int i = 0; i < table_size; ++i) {
            if (other.table[i]) {
                table[i] = other.table[i]->get_copy();
            }
        }
    }
};





struct query {
    int first, second;
    bool equal;
};

void set_console_settings() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

const int size = 1e8 + 1;

hash_map parents = hash_map();
hash_map current_parents = hash_map();
hash_map ranks = hash_map();
hash_map current_ranks = hash_map();

std::vector<query> queries;

std::map<int, std::vector<std::pair<int, int>>> positions;

int comparison_count, first_length, second_length;

int find_set(int node) {
    int value = current_parents.get(node);
    if (value == -1 || value == node) {
        return node;
    }
    int parent = find_set(value);
    current_parents.update(node, parent);
    return parent;
}

void union_sets(int first, int second) {
    first = find_set(first);
    second = find_set(second);
    if (first != second) {
        int rank_first = current_ranks.get(first);
        int rank_second = current_ranks.get(second);

        if (rank_first < rank_second) {
            std::swap(first, second);
            std::swap(rank_first, rank_second);
        }

        current_parents.put(second, first);

        if (rank_first == rank_second) {
            if (rank_first == -1) {
                current_ranks.put(first, 1);
                return;
            }
            current_ranks.increase(first);
        }
        if (rank_first == -1) {
            current_ranks.put(first, 0);
        }
    }
}

bool check_if_same(int first, int second) {
    return find_set(first) == find_set(second);
}

bool is_correct() {
    for (int i = 0; i < queries.size(); ++i) {
        if (!queries[i].equal && check_if_same(queries[i].first, queries[i].second)) {
            return false;
        }
    }
    return true;
}

void add_query(const int first, const int second) {
    int start_index_of_s_in_t = second - first_length - first + 1;
    if (start_index_of_s_in_t > 0 && start_index_of_s_in_t + first_length <= second_length + 1) {
        positions[start_index_of_s_in_t].push_back(std::make_pair(first, second));
    }
}

int main() {
    set_console_settings();

    scanf("%d %d %d", &comparison_count, &first_length, &second_length);

    char symb;

    for (int index = 0; index < comparison_count; ++index) {
        query current = query();
        scanf("%d %c %d", &current.first, &symb, &current.second);
        current.second += first_length;

        for (int i = 0; i < queries.size(); ++i) {
            add_query(queries[i].first, current.second);
            add_query(current.first, queries[i].second);
        }
        add_query(current.first, current.second);

        current.equal = symb == '=';
        if (current.equal) {
            union_sets(current.first, current.second);
        }

        queries.push_back(current);
    }

    if (!is_correct()) {
        printf("%d", 0);
        return 0;
    }

    parents.copy(current_parents);
    ranks.copy(current_ranks);

    int answer = second_length - first_length + 1;

    for (std::map<int, std::vector<std::pair<int, int>>>::iterator it = positions.begin();
        it != positions.end(); ++it) {

        current_parents.copy(parents);
        current_ranks.copy(ranks);

        std::vector<std::pair<int, int>> indexes = it->second;
        for (int i = 0; i < indexes.size(); ++i) {
            union_sets(indexes[i].first, indexes[i].second);
        }

        answer -= !is_correct();
    }

    printf("%d", answer);

    return 0;
}
