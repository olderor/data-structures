#include <iostream> 
#include <fstream> 
#include <vector> 
#include <map>
#include <string>
#include <algorithm> 


struct persistent_array {
public:
    int value;
    persistent_array *left = nullptr;
    persistent_array *right = nullptr;

    static void build(
        persistent_array *root, 
        std::vector<int> &data, 
        const int left, 
        const int right) {
        if (left == right) {
            root->value = data[left];
            return;
        }

        const int mid = (left + right) / 2;
        root->left = new persistent_array();
        root->right = new persistent_array();
        build(root->left, data, left, mid);
        build(root->right, data, mid + 1, right);
    }

    static void update(
        persistent_array *root, 
        const int left, 
        const int right, 
        const int index,
        const int value) {
        if (left == right) {
            root->value = value;
            return;
        }

        const int mid = (left + right) / 2;
        if (index <= mid) {
            persistent_array *new_left = new persistent_array();
            *new_left = *root->left;
            root->left = new_left;
            update(new_left, left, mid, index, value);
        } else {
            persistent_array *new_right = new persistent_array();
            *new_right = *root->right;
            root->right = new_right;
            update(root->right, mid + 1, right, index, value);
        }
    }

    static void set(
        persistent_array *root,
        const int left,
        const int right,
        const int index,
        const int value) {
        if (left == right) {
            root->value = value;
            return;
        }

        const int mid = (left + right) / 2;
        if (index <= mid) {
            set(root->left, left, mid, index, value);
        } else {
            set(root->right, mid + 1, right, index, value);
        }
    }

    static int get(persistent_array *root, const int left, const int right, const int index) {
        if (left == right) {
            return root->value;
        }
        const int mid = (left + right) / 2;
        if (index <= mid) {
            return get(root->left, left, mid, index);
        }
        return get(root->right, mid + 1, right, index);
    }
};


struct persistent_set {
public:
    explicit persistent_set(int size) {
        root[0] = new persistent_array();
        std::vector<int> values(size + 1);
        for (int i = 0; i < size + 1; ++i) {
            values[i] = -1;
        }
        persistent_array::build(root[0], values, 0, size);
        this->size = size + 1;
    }

    int find(const int version, const int element) {
        const int set_index = persistent_array::get(root[version], 0, size - 1, element);

        if (set_index < 0) {
            return element;
        }

        return find(version, set_index);
    }

    bool check(const int version, const int first, const int second) {
        return find(version, first) == find(version, second);
    }

    void union_elements(
        const int version, 
        const int first, 
        const int second, 
        const int new_version) {
        int first_set_index = find(version, first);
        int second_set_idnex = find(version, second);

        persistent_array *new_array = new persistent_array();
        *new_array = *root[version];
        root[new_version] = new_array;

        if (first_set_index != second_set_idnex) {
            int first_value = persistent_array::get(root[version], 0, size - 1, first_set_index);
            int second_value = persistent_array::get(root[version], 0, size - 1, second_set_idnex);

            if (first_value > second_value) {
                std::swap(first_set_index, second_set_idnex);
            }

            persistent_array::set(new_array, 0, size - 1, 
                first_set_index, first_value + second_value);
            persistent_array::update(new_array, 0, size - 1, 
                second_set_idnex, first_set_index);
        }
    }

private:
    persistent_array *root[100001];
    int size = 0;
};




int main() {
    
    std::ifstream cin("input.txt");
    std::ofstream cout("output.txt");


    int elements_count, days_count;
    cin >> elements_count >> days_count;

    persistent_set *set = new persistent_set(elements_count);
    for (int day = 0; day < days_count; ++day) {
        char command;
        cin >> command;
        int version, first, second;
        cin >> version >> first >> second;

        if (command == '+') {
            set->union_elements(version, first, second, day + 1);
        } else {
            if (set->check(version, first, second)) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }



    cin.close();
    cout.close();
    
    return 0;
}
