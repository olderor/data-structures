#include <iostream>
#include <vector>
#include <string>

struct state {
    int link;
    int length;
    std::vector<int> next;

    state(const int link, const int length) : link(link), length(length) {
        next.resize(2);
    }
};

bool data[5000001];
std::vector<state> states;
int max_suffix_palindrome;

void init() {
    states.push_back(state(0, -1));
    states.push_back(state(0, 0));
    max_suffix_palindrome = 1;
}

int add(int index) {
    int current = max_suffix_palindrome;
    while (!(index - states[current].length > 0 &&
      data[index] == data[index - states[current].length - 1])) {
        current = states[current].link;
    }
    if (states[current].next[data[index]] != 0) {
        max_suffix_palindrome = states[current].next[data[index]];
        return 0;
    }
    max_suffix_palindrome = static_cast<int>(states.size());
    states[current].next[data[index]] = max_suffix_palindrome;
    states.push_back(state(0, states[current].length + 2));

    if (states[max_suffix_palindrome].length == 1) {
        states[max_suffix_palindrome].link = 1;
        return 1;
    }
    current = states[current].link;
    while (!(index - states[current].length > 0 &&
      data[index] == data[index - states[current].length - 1])) {
        current = states[current].link;
    }
    states[max_suffix_palindrome].link = states[current].next[data[index]];
    return 1;
}

int main() {
    init();

    for (int i = 0, code; (code = getchar()) != '\n'; ++i) {
        data[i] = code == 'b';
        putchar('0' + add(i));
    }
    return 0;
}
