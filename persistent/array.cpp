#include <iostream> 
#include <fstream> 
#include <vector> 
#include <map>
#include <string>

const int MAX_ELEMENTS_COUNT = 100001;

struct tree {
public:
    int value;
    tree *left = nullptr;
    tree *right = nullptr;

    static void build(tree *root, std::vector<int> &data, int left, int right) {
        if (left == right) {
            root->value = data[left];
            return;
        }

        int mid = (left + right) / 2;
        root->left = new tree();
        root->right = new tree();
        build(root->left, data, left, mid);
        build(root->right, data, mid + 1, right);
    }

    static void update(tree *root, int left, int right, int index, int value) {
        if (left == right) {
            root->value = value;
            return;
        }

        int mid = (left + right) / 2;
        if (index <= mid) {
            tree *new_left = new tree();
            *new_left = *root->left;
            root->left = new_left;
            update(new_left, left, mid, index, value);
        } else {
            tree *new_right = new tree();
            *new_right = *root->right;
            root->right = new_right;
            update(root->right, mid + 1, right, index, value);
        }
    }

    static int get(tree *root, int left, int right, int index) {
        if (left == right) {
            return root->value;
        }
        int mid = (left + right) / 2;
        if (index <= mid) {
            return get(root->left, left, mid, index);
        }
        return get(root->right, mid + 1, right, index);
    }
};



int main() {

    std::ifstream cin("input.txt");
    std::ofstream cout("output.txt");

    int elements_count;
    cin >> elements_count;

    std::vector<int> data;
    data.resize(elements_count);
    for (int i = 0; i < elements_count; ++i) {
        cin >> data[i];
    }

    tree *root[MAX_ELEMENTS_COUNT];
    root[0] = new tree();
    tree::build(root[0], data, 0, elements_count - 1);
    
    int current_version = 1;

    int tests_count;
    cin >> tests_count;
    for (int test_index = 0; test_index < tests_count; ++test_index) {
        std::string command;
        cin >> command;
        if (command[0] == 'c') {
            int version, index, value;
            cin >> version >> index >> value;
            tree *new_tree = new tree();
            *new_tree = *root[version - 1];
            root[current_version++] = new_tree;
            tree::update(new_tree, 0, elements_count - 1, index - 1, value);
        } else {
            int version, index;
            cin >> version >> index;
            cout << tree::get(root[version - 1], 0, elements_count - 1, index - 1);
            cout << std::endl;
        }
    }

    cin.close();
    cout.close();

    return 0;
}
