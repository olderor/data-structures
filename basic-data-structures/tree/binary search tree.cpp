struct tree {
public:
    tree(const int value) {
        this->value = value;
    }

    void add(const int element) {
        if (element == value)
            return;

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

        update_count();
        update_height();
    }

    int second_max() {
        int counter = 0;
        return second_max(counter);
    }

    void print(ostream &cout) {
        if (left) {
            left->print(cout);
        }
        cout << value << " ";
        if (right) {
            right->print(cout);
        }
    }

    void print_leaves(ostream &cout) {
        if (left) {
            left->print_leaves(cout);
        } else if (!right) {
            cout << value << " ";
        }
        if (right) {
            right->print_leaves(cout);
        }
    }

    void print_forks(ostream &cout) {
        if (left) {
            if (left->count > 2 && left->height > 1)
                left->print_forks(cout);
            if (right) {
                cout << value << " ";
            }
        }
        if (right && right->count > 2 && right->height > 1) {
            right->print_forks(cout);
        }
    }

    void print_one_child(ostream &cout) {
        if (left) {

            if (left->height > 1)
                left->print_one_child(cout);

            if (!right) {
                cout << value << " ";
                return;

            } else {

                if (right->height > 1)
                    right->print_one_child(cout);
                return;
            }
        }

        if (right) {
            cout << value << " ";
            if (right->height > 1)
                right->print_one_child(cout);
        }
    }

    int get_size() {
        return count;
    }

    int get_height() {
        return height;
    }

    bool is_balanced() {
        if (left) {
            if (right) {
                if (abs(left->height - right->height) > 1) return false;
                if (!left->is_balanced()) return false;
                return right->is_balanced();
            } else {
                return left->height == 1;
            }
        }
        if (!right) return true;

        return right->height == 1;
    }

private:
    int count = 1;
    int height = 1;
    int value;
    tree *left = nullptr;
    tree *right = nullptr;

private:

    void update_count() {
        count = 1;
        if (left) {
            count += left->count;
        }
        if (right) {
            count += right->count;
        }
    }

    void update_height() {
        height = 0;
        if (left) {
            height = left->height + 1;
        }
        if (right) {
            height = max(height, right->height + 1);
        }
    }

    int second_max(int &counter) {
        if (counter >= 2)
            return -1;

        if (right) {
            int v = right->second_max(counter);
            if (v != -1)
                return v;
        }
        ++counter;
        if (counter == 2) {
            return value;
        }
        if (left) {
            return left->second_max(counter);
        }
        return -1;
    }
};
