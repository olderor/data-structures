#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct link_cut_tree {
public:
    explicit link_cut_tree(int id) {
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
        while (!is_root(tree))
        {
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

    static int get_size(link_cut_tree *tree) {
        if (!tree) {
            return 0;
        }
        return tree->size;
    }

private:
    link_cut_tree *parent = nullptr;
    link_cut_tree *left = nullptr;
    link_cut_tree *right = nullptr;
    int size = 1;
    int index;
    bool rev = false;

    static void rotate(link_cut_tree *tree) {
        link_cut_tree *tree_parent = tree->parent;
        link_cut_tree *grand_parent = tree_parent->parent;

        int tree_dir = parent_direction(tree);
        int parent_dir = parent_direction(tree_parent);

        if (tree_dir == 0) {
            connect(tree->left, tree_parent, tree_dir);
        } else {
            connect(tree->right, tree_parent, tree_dir);
        }
        connect(tree_parent, tree, tree_dir ^ 1);
        connect(tree, grand_parent, parent_dir);

        update(tree_parent);
    }

    static void connect(link_cut_tree *child, link_cut_tree *parent, int dir) {
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

        if (tree->left) {
            tree->left->rev ^= tree->rev;
        }
        if (tree->right) {
            tree->right->rev ^= tree->rev;
        }

        if (tree->rev) {
            std::swap(tree->left, tree->right);
        }

        tree->rev = false;
    }

    static int parent_direction(link_cut_tree *tree) {
        if (is_root(tree)) return -1;
        return tree->parent->right == tree ? 0 : 1;
    }

    static bool is_root(link_cut_tree *tree) {
        if (!tree->parent) {
            return true;
        }
        return tree->parent->left != tree && tree->parent->right != tree;
    }

    static void update(link_cut_tree *tree) {
        tree->size = 1 + get_size(tree->left) + get_size(tree->right);
    }
};

struct graph {
public:
    explicit graph(int size) {
        this->size = size;
        for (int i = 0; i < size; ++i) {
            trees.push_back(new link_cut_tree(i + 1));
        }
    }

    void add_edge(int first, int second) {
        link_cut_tree::merge(trees[first], trees[second]);
    }

    void cut(int first, int second) {
        link_cut_tree::evert(trees[first]);
        link_cut_tree::expose(trees[second]);
        link_cut_tree::cut(trees[second]);
    }

    int get(int first, int second) {
        if (!link_cut_tree::compare(trees[first], trees[second])) {
            return -1;
        }
        link_cut_tree::evert(trees[second]);
        link_cut_tree::expose(trees[first]);
        link_cut_tree::splay(trees[first]);
        return link_cut_tree::get_size(trees[first]) - 1;
    }

private:
    int size;
    std::vector<link_cut_tree*> trees;
};


int main() {

    int size, queries;
    std::cin >> size >> queries;

    graph *cur_graph = new graph(size);
    int first, second;
    std::string command;
    for (int query_i = 0; query_i < queries; ++query_i) {
        std::cin >> command >> first >> second;
        if (command[0] == 'l') {
            cur_graph->add_edge(first - 1, second - 1);
            continue;
        }
        if (command[0] == 'c') {
            cur_graph->cut(first - 1, second - 1);
            continue;
        }
        std::cout << cur_graph->get(first - 1, second - 1) << "\n";
    }

    return 0;
}
