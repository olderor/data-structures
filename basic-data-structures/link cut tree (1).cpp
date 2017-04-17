#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct link_cut_tree {
public:
    explicit link_cut_tree(long long id) {
        index = id;
    }

    static bool compare(link_cut_tree *tree, link_cut_tree *other_tree) {
        evert(tree);
        expose(other_tree);
        while (tree->parent) {
            tree = tree->parent;
        }
        return tree == other_tree;
    }

    static void merge(link_cut_tree *tree, link_cut_tree *other_tree) {
        evert(tree);
        connect(tree, other_tree, -1);
    }

    static void evert(link_cut_tree *tree) {
        expose(tree);
        tree->rev = true;
    }

    static void expose(link_cut_tree *tree) {
        link_cut_tree *current = tree;
        link_cut_tree *new_right = nullptr;
        while (current) {
            splay(current);
            current->right = new_right;
            new_right = current;
            current = current->parent;
        }
        splay(tree);
    }

    static void cut(link_cut_tree *tree) {
        tree->left->parent = nullptr;
        tree->left = nullptr;
    }

    static void splay(link_cut_tree *tree) {
        push(tree);
        while (!is_root(tree)) {
            push(tree->parent->parent);
            push(tree->parent);
            push(tree);
            if (is_root(tree->parent)) {
                rotate(tree);
            } else {
                if (parent_direction(tree) == parent_direction(tree->parent)) {
                    rotate(tree->parent);
                    rotate(tree);
                } else {
                    rotate(tree);
                    rotate(tree);
                }
            }
        }
        update(tree);
    }

    static long long get_size(link_cut_tree *tree) {
        if (!tree) {
            return 0;
        }
        return tree->size;
    }

    static long long get_value(link_cut_tree *tree) {
        expose(tree);
        return get_sub_value(tree);
    }

    static void add_value(link_cut_tree *tree, long long value) {
        expose(tree);
        tree->value += value;
    }

private:
    link_cut_tree *parent = nullptr;
    link_cut_tree *left = nullptr;
    link_cut_tree *right = nullptr;
    long long size = 1;
    long long index;
    long long value = 0;
    long long delta = 0;
    long long sub_value = 0;

    bool rev = false;

    static void rotate(link_cut_tree *tree) {
        link_cut_tree *tree_parent = tree->parent;
        link_cut_tree *grand_parent = tree_parent->parent;
        long long tree_dir = parent_direction(tree);
        long long parent_dir = parent_direction(tree_parent);
        if (tree_dir == 0) {
            connect(tree->left, tree_parent, tree_dir);
        } else {
            connect(tree->right, tree_parent, tree_dir);
        }
        connect(tree_parent, tree, tree_dir ^ 1);
        connect(tree, grand_parent, parent_dir);
        update(tree_parent);
    }

    static void connect(link_cut_tree *child, link_cut_tree *parent, long long dir) {
        if (parent && dir != -1) {
            if (dir == 1) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        }
        if (child) {
            child->parent = parent;
        }
    }

    static void push(link_cut_tree *tree) {
        if (!tree) {
            return;
        }
        tree->value = std::max(tree->value, tree->delta);
        tree->sub_value = std::max(tree->value, tree->delta);
        if (tree->left) {
            tree->left->rev ^= tree->rev;
            tree->left->delta = std::max(tree->left->delta, tree->delta);
        }
        if (tree->right) {
            tree->right->rev ^= tree->rev;
            tree->right->delta = std::max(tree->right->delta, tree->delta);
        }
        if (tree->rev) {
            std::swap(tree->left, tree->right);
        }
        tree->delta = 0;
        tree->rev = false;
    }

    static long long parent_direction(link_cut_tree *tree) {
        if (is_root(tree)) {
            return -1;
        }
        return tree->parent->right == tree ? 0 : 1;
    }

    static bool is_root(link_cut_tree *tree) {
        if (!tree->parent) {
            return true;
        }
        return tree->parent->left != tree && tree->parent->right != tree;
    }

    static void update(link_cut_tree *tree) {
        tree->sub_value = tree->value + tree->delta;
        tree->sub_value = std::max(tree->sub_value, get_sub_value(tree->left));
        tree->sub_value = std::max(tree->sub_value, get_sub_value(tree->right));
        tree->size = 1 + get_size(tree->left) + get_size(tree->right);
    }

    static long long get_sub_value(link_cut_tree *tree) {
        if (!tree) {
            return 0;
        }
        return std::max(tree->sub_value, tree->delta);
    }
};

struct graph {
public:
    explicit graph(long long size) {
        this->size = size;
        for (long long tree_i = 0; tree_i < size; ++tree_i) {
            trees.push_back(new link_cut_tree(tree_i + 1));
        }
    }

    void add_edge(long long first, long long second) {
        link_cut_tree::merge(trees[first], trees[second]);
    }

    void cut(long long first, long long second) {
        link_cut_tree::evert(trees[first]);
        link_cut_tree::expose(trees[second]);
        link_cut_tree::cut(trees[second]);
    }

    long long get(long long first, long long second) {
        if (!link_cut_tree::compare(trees[first], trees[second])) {
            return -1;
        }
        link_cut_tree::evert(trees[second]);
        link_cut_tree::expose(trees[first]);
        link_cut_tree::splay(trees[first]);
        return link_cut_tree::get_size(trees[first]) - 1;
    }

    long long get_value(long long first, long long second) {
        link_cut_tree::evert(trees[first]);
        return link_cut_tree::get_value(trees[second]);
    }

    void add_value(long long first, long long value) {
        link_cut_tree::add_value(trees[first], value);
    }

private:
    long long size;
    std::vector<link_cut_tree*> trees;
};

int main() {

    long long size, queries;
    std::cin >> size;

    graph *cur_graph = new graph(size);
    long long first, second;
    for (long long index_i = 0; index_i < size - 1; ++index_i) {
        std::cin >> first >> second;
        cur_graph->add_edge(first - 1, second - 1);
    }
    
    std::cin >> queries;
    char command;
    for (long long query_i = 0; query_i < queries; ++query_i) {
        std::cin >> command >> first >> second;
        if (command == 'G') {
            std::cout << cur_graph->get_value(first - 1, second - 1) << "\n";
            continue;
        }
        cur_graph->add_value(first - 1, second);
    }

    return 0;
}
