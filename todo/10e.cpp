// https://contest.yandex.ru/contest/4612/problems/E/

//
//  main.cpp
//  advanced5-5
//
//  Created by olderor on 17.06.17.
//  Copyright © 2017 Bohdan Yevchenko. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

std::vector<int> prefix_func;
std::string str;
int str_len;

int best_prefix_func_index;

void calc_prefix_function() {
    prefix_func[0] = 0;
    best_prefix_func_index = 0;

    int matched_prefix, candidate;
    for (int current = 1; current < str_len; ++current) {
        matched_prefix = current - 1;
        candidate = prefix_func[matched_prefix];
        while (candidate != 0 &&
          str[str_len - current - 1] != str[str_len - candidate - 1]) {
            matched_prefix = prefix_func[matched_prefix] - 1;
            candidate = prefix_func[matched_prefix];
        }

        if (candidate == 0) {
            prefix_func[current] =
              str[str_len - current - 1] == str[str_len - 1];
        } else {
            prefix_func[current] = candidate + 1;
        }
        if (prefix_func[best_prefix_func_index] < prefix_func[current]) {
            best_prefix_func_index = current;
        }
    }
}

struct command {
    bool repeatable;
    char character;
    int start, count, part_length;
    command() {
    }
    explicit command(const char character)
      : repeatable(false), character(character) {
    }
    command(const int start, const int count, const int part_length)
      : repeatable(true), start(start), count(count), part_length(part_length) {
    }
};

std::vector<command> commands;
std::vector<bool> should_repeat;
std::vector<int> dynamic;

int main() {
    std::cin >> str;
    str_len = static_cast<int>(str.length());
    should_repeat.resize(str_len);
    dynamic.resize(str_len + 1);
    prefix_func.resize(str_len);
    commands.resize(str_len);
    int new_str_len;
    while (str_len) {
        calc_prefix_function();

        const int start = str_len - best_prefix_func_index - 1;
        const int part_length =
          str_len - prefix_func[best_prefix_func_index] - start;
        const int count = str_len - start - part_length;

        new_str_len = str_len - 1;
        if (dynamic[new_str_len] == 0 ||
          dynamic[new_str_len] > dynamic[str_len] + 1) {
            should_repeat[new_str_len] = false;
            dynamic[new_str_len] = dynamic[str_len] + 1;
        }
        if (prefix_func[best_prefix_func_index] != 0) {
            new_str_len = start + part_length;
            if (dynamic[new_str_len] == 0 ||
              dynamic[new_str_len] >= dynamic[str_len] + 5) {
                commands[new_str_len] = command(start, count, part_length);
                should_repeat[new_str_len] = true;
                dynamic[new_str_len] = dynamic[str_len] + 5;
            }
        }
        --str_len;
    }

    std::vector<command> answer;
    int index = 0;
    while (index < str.length()) {
        if (should_repeat[index]) {
            answer.push_back(commands[index]);
            index += commands[index].count;
        } else {
            answer.push_back(command(str[index]));
            ++index;
        }
    }

    std::cout << dynamic[0] << " " << answer.size() << '\n';
    for (int i = 0; i < answer.size(); ++i) {
        if (answer[i].repeatable) {
            std::cout << answer[i].start + 1 << " " << answer[i].count << '\n';
        } else {
            std::cout << answer[i].character << '\n';
        }
    }

    return 0;
}
