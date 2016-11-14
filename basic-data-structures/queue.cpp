class queue_exception : public exception {
    virtual const char* what() const throw() {
        return "error";
    }
} queue_ex;

struct queue {
public:

    void push(int value) {

        ++count;

        if (!root) {
            root = new node(value);
            return;
        }
        node *current = root;
        while (current->next) {
            current = current->next;
        }
        current->next = new node(value);
    }

    int pop() {
        if (count == 0) {
            throw queue_ex;
        }
        --count;
        int value = root->value;
        root = root->next;
        return value;
    }

    int front() {
        if (count == 0) {
            throw queue_ex;
        }
        return root->value;
    }

    int size() {
        return count;
    }

    void clear() {
        root = nullptr;
        count = 0;
    }

private:

    int count = 0;

    struct node {
    public:

        node(int value) {
            this->value = value;
        }

        int value;
        node *next = nullptr;
    };

    node *root = nullptr;
};
