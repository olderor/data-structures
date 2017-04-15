#include <iostream>
#include <vector>
#include <algorithm>

#pragma warning(disable:4996)

// Used for solving the problem.
struct solver {
public:
    // Get answer to the problem - minimum price to fix wall.
    const int get_matching();

    // Stores bigraph. Each vertex stores their own number of connected vertex.
    std::vector < std::vector<int> > bigraph;
    // Matching in the graph theory.
    std::vector<int> independent_edge_set;
    // Used for checking if vertex can be used in the new matching.
    std::vector<bool> used;
    // Define if vertex is used in the random matching filling.
    std::vector<bool> used_temp;

    // Number of vertex in the first part of bigraph.
    int first_size = 0;
    // Number of vertex in the second part of bigraph.
    int second_size = 0;

    // Kuhn algorithm to improve matching for the particular vertex.
    const bool find_maximum_matching_for_vertex(const int vertex);
    // Set matching randomly.
    const void set_random_matching();
    // Try to improve matching from the current state.
    const void find_maximum_matching();
    // Get matching size.
    const int get_connections_count() const;
};

const int solver::get_matching() {
    set_random_matching();
    find_maximum_matching();
    const int connections = get_connections_count();

    return connections;
}

const bool solver::find_maximum_matching_for_vertex(const int vertex) {
    if (used[vertex]) {
        return false;
    }

    used[vertex] = true;

    for (int i = 0; i < bigraph[vertex].size(); ++i) {
        const int to = bigraph[vertex][i];
        if (independent_edge_set[to] == -1) {
            independent_edge_set[to] = vertex;
            return true;
        }
        if (find_maximum_matching_for_vertex(independent_edge_set[to])) {
            independent_edge_set[to] = vertex;
            return true;
        }
    }

    return false;
}

const void solver::set_random_matching() {
    independent_edge_set = std::vector<int>(second_size, -1);
    used_temp = std::vector<bool>(first_size);
    for (int i = 0; i < first_size; ++i) {
        for (int j = 0; j < bigraph[i].size(); ++j) {
            if (independent_edge_set[bigraph[i][j]] == -1) {
                independent_edge_set[bigraph[i][j]] = i;
                used_temp[i] = true;
                break;
            }
        }
    }
}

const void solver::find_maximum_matching() {
    for (int i = 0; i < first_size; ++i) {
        if (used_temp[i]) {
            continue;
        }
        used = std::vector<bool>(first_size, false);
        find_maximum_matching_for_vertex(i);
    }
}

const int solver::get_connections_count() const {
    int connections_count = 0;
    for (int i = 0; i < second_size; ++i) {
        if (independent_edge_set[i] != -1) {
            ++connections_count;
        }
    }
    return connections_count;
}

long long speed;

struct timing {
    long long minutes;
    long long coord_x;
    long long coord_y;
};

bool check(timing *first, timing *second) {
    long long dist = (first->coord_x - second->coord_x) * (first->coord_x - second->coord_x) +
        (first->coord_y - second->coord_y) * (first->coord_y - second->coord_y);
    long long time_dif = first->minutes - second->minutes;
    if (time_dif < 0) {
        time_dif = -time_dif;
    }
    return dist * 60 * 60 <= speed * speed * time_dif * time_dif;
}

bool time_cmp(timing *first, timing *second) {
    return first->minutes < second->minutes;
}

int main() {
    int vertices_count;
    scanf("%d %lld", &vertices_count, &speed);

    std::vector<timing*> timings(vertices_count);

    int hours, minutes, coord_x, coord_y;
    for (int first_index = 0; first_index < vertices_count; ++first_index) {
        timings[first_index] = new timing();
        scanf("%d:%d %d %d", &hours, &minutes, &coord_x, &coord_y);

        timings[first_index]->minutes = hours * 60 + minutes;
        timings[first_index]->coord_x = coord_x;
        timings[first_index]->coord_y = coord_y;
    }

    sort(timings.begin(), timings.end(), time_cmp);

    solver sv = solver();
    sv.first_size = vertices_count;
    sv.second_size = vertices_count;
    sv.bigraph.resize(vertices_count);
    for (int first_index = 0; first_index < vertices_count; ++first_index) {
        for (int second_index = 0; second_index < first_index; ++second_index) {
            if (check(timings[first_index], timings[second_index])) {
                sv.bigraph[second_index].push_back(first_index);
            }
        }
    }

    int matching_size = sv.get_matching();

    printf("%d\n", matching_size + vertices_count - matching_size * 2);
    return 0;
}
