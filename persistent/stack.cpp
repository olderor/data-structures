#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>

struct stack {
public:
    stack() {
        query new_query;
        new_query.value = 0;
        new_query.sum = 0;
        new_query.previous = 0;
        queries.push_back(new_query);
    }

    void push(const int version, const int value) {
        query new_query;
        new_query.value = value;
        new_query.previous = version;
        new_query.sum = value + queries[version].sum;
        queries.push_back(new_query);
    }
    int pop(const int version) {
        query q_curr = queries[version];
        query q_prev = queries[q_curr.previous];
        push(q_prev.previous, q_prev.value);
        return q_curr.value;
    }

    unsigned long long get_sum() {
        unsigned long long sum = 0;
        for (int i = 0; i < queries.size(); ++i) {
            sum += queries[i].sum;
        }
        return sum;
    }

private:
    struct query {
        int value;
        int previous;
        unsigned long long sum;
    };
    std::vector<query> queries;
};


int main() {

    std::ifstream cin("input.txt");
    std::ofstream cout("output.txt");

    stack *towers = new stack();

    int movings_count;
    cin >> movings_count;
    for (int i = 0; i < movings_count; ++i) {
        int version, points;
        cin >> version >> points;
        if (points == 0) {
            towers->pop(version);
        } else {
            towers->push(version, points);
        }
    }

    cout << towers->get_sum();

    cin.close();
    cout.close();

    return 0;
}
