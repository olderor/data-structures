struct tree {
public:

    tree() {}

    tree(const ll value) {
        this->value = value;
        this->was_set = true;
    }

    ll pop_min() {
        if (!left) {
            ll res = value;
            if (right) {
                value = right->value;
                was_set = true;
                left = right->left;
                right = right->right;
                return res;
            }
            was_set = false;
            return res;
        }

        tree *current = this;
        while (current->left->left) {
            current = current->left;
        }
        ll res = current->left->value;
        if (current->left->right) {
            current->left = current->left->right;
            return res;
        }
        current->left = nullptr;
        return res;
    }

    ll pop_max() {
        if (!right) {
            ll res = value;
            if (left) {
                value = left->value;
                was_set = true;
                right = left->right;
                left = left->left;
                return res;
            }
            was_set = false;
            return res;
        }

        tree *current = this;
        while (current->right->right) {
            current = current->right;
        }
        ll res = current->right->value;
        if (current->right->left) {
            current->right = current->right->left;
            return res;
        }
        current->right = nullptr;
        return res;
    }

    void add(const ll element) {

        if (!was_set) {
            value = element;
            was_set = true;
            return;
        }

        if (element < value) {
            if (!left) {
                left = new tree(element);
            } else {
                left->add(element);
            }
        } else {
            if (!right) {
                right = new tree(element);
            } else {
                right->add(element);
            }
        }
    }

private:
    ll value;
    bool was_set = false;
    tree *left = nullptr;
    tree *right = nullptr;
};
