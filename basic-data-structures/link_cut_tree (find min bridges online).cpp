#include <iostream> 
#include <fstream> 
#include <vector>
#include <algorithm>

struct link_cut_tree {
public:
    explicit link_cut_tree(bool is_vertex) {
        if (is_vertex)
            return;

        bridges = 1;
        is_bridge = true;
    }

    static bool compare(link_cut_tree *tree, link_cut_tree *other_tree) {
        evert(tree);
        expose(other_tree);
        while (tree->parent) {
            tree = tree->parent;
        }
        return tree == other_tree;
    }

    int bridges_count() {
        if (circle) {
            return 0;
        }
        return bridges;
    }

    void set_circle(bool circle) {
        this->circle = circle;
    }

    static void merge(link_cut_tree *tree, link_cut_tree *other_tree) {
        expose(tree);
        expose(other_tree);
        connect(tree, other_tree, 0);
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
            update_bridges(current);
            current = current->parent;
        }
        splay(tree);
    }

private:
    link_cut_tree *parent = nullptr;
    link_cut_tree *left = nullptr;
    link_cut_tree *right = nullptr;

    bool rev = false;
    bool circle = false;
    bool is_bridge = false;
    int bridges = 0;

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
    }

    static void connect(link_cut_tree *child, link_cut_tree *parent, int dir) {
        if (parent && dir != -1) {
            if (dir == 1) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            update_bridges(parent);
        }
        if (child) {
            child->parent = parent;
        }
    }

    static void update_bridges(link_cut_tree *tree) {
        if (!tree) {
            return;
        }
        tree->bridges = tree->is_bridge;
        if (tree->left) {
            tree->bridges += tree->left->bridges_count();
        }
        if (tree->right) {
            tree->bridges += tree->right->bridges_count();
        }
    }

    static void push(link_cut_tree *tree) {
        if (!tree) {
            return;
        }

        if (tree->left) {
            tree->left->rev ^= tree->rev;
            if (tree->circle) {
                tree->left->circle = true;
            }
        }
        if (tree->right) {
            tree->right->rev ^= tree->rev;
            if (tree->circle) {
                tree->right->circle = true;
            }
        }

        if (tree->rev) {
            std::swap(tree->left, tree->right);
        }
        if (tree->circle) {
            tree->is_bridge = false;
        }
        tree->rev = false;
        tree->circle = false;

        update_bridges(tree);
    }

    static int parent_direction(link_cut_tree *tree) {
        if (is_root(tree)) return -1;
        return tree->parent->right == tree ? 0 : 1;
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
        update_bridges(tree);
    }

    static bool is_root(link_cut_tree *tree) {
        if (!tree->parent) {
            return true;
        }
        return tree->parent->left != tree && tree->parent->right != tree;
    }
};

struct graph {
public:
    explicit graph(int size) {
        this->size = size;
        for (int i = 0; i < size; ++i) {
            trees.push_back(new link_cut_tree(true));
        }
    }

    void add_edge(int first, int second) {
        if (link_cut_tree::compare(trees[first], trees[second])) {
            link_cut_tree::evert(trees[first]);
            link_cut_tree::expose(trees[second]);
            bridges -= trees[second]->bridges_count();
            trees[second]->set_circle(true);
            return;
        }

        link_cut_tree *edge = new link_cut_tree(false);
        link_cut_tree::evert(trees[first]);
        link_cut_tree::merge(trees[first], edge);
        link_cut_tree::merge(edge, trees[second]);
        ++bridges;
    }

    int bridges_count() {
        return bridges;
    }

private:
    int size;
    std::vector<link_cut_tree*> trees;
    int bridges = 0;
};


int main() {

    std::ifstream cin("input.txt");
    std::ofstream cout("output.txt");

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int size, edges;
    cin >> size >> edges;

    graph *cur_graph = new graph(size);
    int first, second;
    for (int i = 0; i < edges; ++i) {
        cin >> first >> second;
        cur_graph->add_edge(first - 1, second - 1);
    }

    int test_count;
    cin >> test_count;
    for (int i = 0; i < test_count; ++i) {
        cin >> first >> second;
        cur_graph->add_edge(first - 1, second - 1);
        cout << cur_graph->bridges_count() << "\n";
    }

    cin.close();
    cout.close();

    return 0;
}
