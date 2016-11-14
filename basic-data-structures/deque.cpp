class deque_exception : public exception {
    virtual const char* what() const throw() {
        return "error";
    }
} deque_ex;

struct deque {
public:

    void push_front(int value) {
        left_index = (left_index - 1 + 101) % 101;
        data[left_index] = value;
    }

    void push_back(int value) {
        data[right_index] = value;
        right_index = (right_index + 1) % 101;
    }

    int pop_front() {
        if (size() == 0) {
            throw deque_ex;
        }
        int value = data[left_index];
        left_index = (left_index + 1) % 101;
        return value;
    }

    int pop_back() {
        if (size() == 0) {
            throw deque_ex;
        }
        right_index = (right_index - 1 + 101) % 101;
        return data[right_index];
    }

    int front() {
        if (size() == 0) {
            throw deque_ex;
        }
        return data[left_index];
    }

    int back() {
        if (size() == 0) {
            throw deque_ex;
        }
        return data[(right_index - 1 + 101) % 101];
    }

    int size() {
        return (right_index - left_index + 101) % 101;
    }

    void clear() {
        left_index = right_index;
    }

private:
    int data[101];
    int left_index = 0;
    int right_index = 0;
};
