#include <algorithm>
#include <vector>
#include <map>

#pragma warning(disable:4996)

int main() {
    int size, queries;
    scanf("%d %d", &size, &queries);
    std::vector<int> values(size);

    int len = sqrt(size) + 1;
    std::vector<int> subsegments(len);

    for (int i = 0; i < size; ++i) {
        scanf(" %d", &values[i]);
        subsegments[i / len] += values[i];
    }

    for (int query = 0; query < queries; ++query) {
        char command;
        scanf("%c", &command);
        scanf("%c", &command);
        if (command == '+') {
            int index, difference;
            scanf("%d %d", &index, &difference);
            --index;
            values[index] += difference;
            subsegments[index / len] += difference;
            continue;
        }

        int left, right;
        scanf("%d %d", &left, &right);
        --left;
        --right;
        int sum = 0;
        int start_block = left / len;
        int end_block = right / len;
        if (start_block == end_block) {
            for (int i = left; i <= right; ++i) {
                sum += values[i];
            }
        } else {
            for (int i = left, end = (start_block + 1) * len - 1; i <= end; ++i) {
                sum += values[i];
            }
            for (int i = start_block + 1; i <= end_block - 1; ++i) {
                sum += subsegments[i];
            }
            for (int i = end_block * len; i <= right; ++i) {
                sum += values[i];
            }
        }
        printf("%d\n", sum);
    }

    return 0;
}
