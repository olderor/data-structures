// http://acm.timus.ru/problem.aspx?space=1&num=2040

//
//  main.cpp
//  advanced-palindrome2
//
//  Created by olderor on 20.06.17.
//  Copyright © 2017 Bohdan Yevchenko. All rights reserved.
//

#include <stdio.h>

int link[5000005];
int length[5000005];
int next[10000005];

char data[5000001];
int states_size = 0;
int max_suffix_palindrome = 1;
int odd = 0;
int even = 1;
int current;

int main() {
    length[states_size++] = -1;
    ++states_size;
    max_suffix_palindrome = 1;
    int index = -1;
    scanf("%s", data);
    while (data[++index]) {
        data[index] -= 'a';
        current = max_suffix_palindrome;
        while (data[index] != data[index - length[current] - 1] ||
          index - length[current] == 0) {
            current = link[current];
        }
        if (next[current + 5000001 * data[index]] != 0) {
            max_suffix_palindrome = next[current + 5000001 * data[index]];
            putchar('0');
        } else {
            max_suffix_palindrome = states_size;
            next[current + 5000001 * data[index]] = max_suffix_palindrome;
            length[states_size++] = length[current] + 2;

            if (length[max_suffix_palindrome] == 1) {
                link[max_suffix_palindrome] = 1;
            } else {
                current = link[current];
                while (data[index] != data[index - length[current] - 1] ||
                  index - length[current] == 0) {
                    current = link[current];
                }
                link[max_suffix_palindrome] =
                  next[current + 5000001 * data[index]];
            }
            putchar('1');
        }
    }
    return 0;
}
