// https://contest.yandex.ru/contest/3476/problems/B/
#include <iostream> 
#include <istream> 
#include <fstream> 
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <math.h>
#include <utility>
#include <ctime>

#pragma warning(disable:4996)

struct query {
    int count, from, to;
};

int cards[2][100001];
query queries[225];
int indexes[100001];
int end_indexes[100001];

int main() {
    std::ios_base::sync_with_stdio(false);


    int cards_count, queries_count;
    scanf("%d %d", &cards_count, &queries_count);

    int query_index = 0;
    const int part_length = sqrt(queries_count);

    for (int i = 0; i < cards_count; ++i) {
        cards[0][i] = 1;
        indexes[i] = i;
    }

    bool flag = false;

    while (query_index < queries_count) {
        int length = part_length;
        if (length > queries_count - query_index) {
            length = queries_count - query_index;
        }

        for (int i = 0; i < length; ++i) {
            scanf("%d %d %d", &queries[i].count, &queries[i].from, &queries[i].to);
            --queries[i].from;
            --queries[i].to;
        }

        for (int card_index = 0; card_index < cards_count; ++card_index) {
            if (cards[flag][card_index] == 0) {
                continue;
            }

            int current_from = card_index;
            int right_index = cards_count - 1;
            int current_count = cards_count - current_from;

            for (int query_i = 0; query_i < length; ++query_i) {
                const int query_right_index = queries[query_i].from + queries[query_i].count - 1;
                if (queries[query_i].from <= current_from &&
                    current_from <= query_right_index) {
                    const int dif = current_from + current_count - query_right_index - 1;
                    current_from = queries[query_i].to + current_from - queries[query_i].from;
                    if (dif < 0) {
                        continue;
                    }
                    current_count -= dif;
                    right_index -= dif;
                } else if (current_from <= queries[query_i].from &&
                    queries[query_i].from <= current_from + current_count) {
                    const int dif = current_from + current_count - queries[query_i].from;
                    current_count -= dif;
                    right_index -= dif;
                }
            }
            for (int i = card_index; i <= right_index; ++i) {
                cards[!flag][current_from + i - card_index] += cards[flag][i];
                cards[flag][i] = 0;
                end_indexes[i] = current_from + i - card_index;
            }
            card_index = right_index;
        }
        for (int i = 0; i < cards_count; ++i) {
            indexes[i] = end_indexes[indexes[i]];
        }
        flag = !flag;
        query_index += length;
    }

    for (int i = 0; i < cards_count; ++i) {
        printf("%d ", indexes[i] + 1);
    }

    return 0;
}
