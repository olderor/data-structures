class stack_exception : public exception {
    virtual const char* what() const throw() {
        return "error";
    }
} st_ex;

struct stack {
public:
    
    void push(int value) {
        data[index++] = value;
    }

    int pop() {
        if (index == 0) {
            throw st_ex;
        }
        return data[--index];
    }

    int back() {
        if (index == 0) {
            throw st_ex;
        }
        return data[index - 1];
    }

    int size() {
        return index;
    }

    void clear() {
        index = 0;
    }

private:
    int data[101];
    int index = 0;
};
