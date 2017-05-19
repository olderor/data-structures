// https://contest.yandex.ru/contest/4545/problems/A/
// Copyright 2017 olderor (Yevchenko Bohdan)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#pragma warning(disable:4996)

std::string original;
std::vector<unsigned long long> pows, hashes;

const unsigned long long MODULO = 184467440;

void calc_pows(const int length) {
    pows.resize(length);
    pows[0] = 1;
    for (int i = 1; i < length; ++i) {
        pows[i] = (pows[i - 1] * 31) % MODULO;
        // pows[i] = pows[i - 1] * 31;
    }
}

void calc_hashes(const int length) {
    hashes.resize(length + 1);
    hashes[0] = 0;
    for (int i = 0; i < length; ++i) {
        hashes[i + 1] = ((((original[i] - 'a' + 1) * pows[i]) % MODULO) + hashes[i]) % MODULO;
        // hashes[i + 1] = (original[i] - 'a' + 1) * pows[i] + hashes[i];
    }
}

unsigned long long get_hash(const int left, const int right) {
    return (hashes[right] + MODULO - hashes[left - 1]) % MODULO;
    // return hashes[right] - hashes[left - 1];
}

int main() {
    int queries_count;

    std::cin >> original >> queries_count;

    calc_pows(original.length());
    calc_hashes(original.length());

    int first_left, first_right, second_left, second_right;
    unsigned long long first_hash, second_hash;
    for (int query = 0; query < queries_count; ++query) {
        std::cin >> first_left >> first_right >> second_left >> second_right;

        if (first_left > second_left || first_left == second_left && first_right > second_right) {
            std::swap(first_left, second_left);
            std::swap(first_right, second_right);
        }

        first_hash = (get_hash(first_left, first_right) * pows[second_left - first_left]) % MODULO;
        // first_hash = get_hash(first_left, first_right) * pows[second_left - first_left];
        second_hash = get_hash(second_left, second_right);

        if (first_hash == second_hash) {
            printf("Yes\n");
        } else {
            printf("No\n");
        }
    }

    return 0;
}
