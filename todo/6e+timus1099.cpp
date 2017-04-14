#include <iostream>
#include <vector>

int vertices_count;
int matching_size;
int vertices[500];
int vertices_size;
int matching[500];
std::vector<int> graph[500];
int edges_for_rand_vertex[500];
int previous[500];
int visited[500];
int current_blossom[500];
int blossoms_visited[500];
int queue[500];
int queue_size;
int queue_index;
int in_new_blossom[500];

void get_possible_vertices() {
    int edge_index;
    vertices_size = 0;
    for (int vertex = 0; vertex < vertices_count; ++vertex) {
        if (matching[vertex] != -1) {
            continue;
        }
        for (edge_index = 0; edge_index < graph[vertex].size(); ++edge_index) {
            if (matching[graph[vertex][edge_index]] == -1) {
                vertices[vertices_size++] = vertex;
                break;
            }
        }
    }
}

void connect_random_vertices() {
    int rand_vertex = vertices[rand() % vertices_size];
    int edges_for_rand_vertex_size = 0;

    for (int i = 0; i < graph[rand_vertex].size(); ++i) {
        if (matching[graph[rand_vertex][i]] == -1) {
            edges_for_rand_vertex[edges_for_rand_vertex_size++] = graph[rand_vertex][i];
        }
    }

    int rand_vertex_to_connect = edges_for_rand_vertex[rand() % edges_for_rand_vertex_size];
    ++matching_size;
    matching[rand_vertex] = rand_vertex_to_connect;
    matching[rand_vertex_to_connect] = rand_vertex;
}


void set_random_matching() {
    get_possible_vertices();
    while (vertices_size) {
        connect_random_vertices();
        get_possible_vertices();
    }
}

int blossom_vertex;
int lca(int first, int second) {
    for (int i = 0; i < vertices_count; ++i) {
        blossoms_visited[i] = 0;
    }

    blossom_vertex = current_blossom[first];
    blossoms_visited[blossom_vertex] = 1;

    while (matching[blossom_vertex] != -1 && previous[matching[blossom_vertex]] != -1) {
        blossom_vertex = current_blossom[previous[matching[blossom_vertex]]];
        blossoms_visited[blossom_vertex] = 1;
    }

    blossom_vertex = current_blossom[second];
    while (blossoms_visited[blossom_vertex] == 0) {
        blossom_vertex = current_blossom[previous[matching[blossom_vertex]]];
    }
    return blossom_vertex;
}

void reorder_blossom(int vertex, int connected_vertex, int new_blossom) {
    int current_vertex = vertex;
    int current_connected_vertex = connected_vertex;

    while (current_blossom[current_vertex] != new_blossom) {
        in_new_blossom[current_blossom[matching[current_vertex]]] = true;
        in_new_blossom[current_blossom[current_vertex]] = true;

        previous[current_vertex] = current_connected_vertex;
        current_connected_vertex = matching[current_vertex];
        current_vertex = previous[current_connected_vertex];
    }
}

void make_new_blossom(int vertex, int connected_vertex) {
    int new_blossom = lca(vertex, connected_vertex);

    for (int i = 0; i < vertices_count; ++i) {
        in_new_blossom[i] = 0;
    }

    reorder_blossom(vertex, connected_vertex, new_blossom);
    reorder_blossom(connected_vertex, vertex, new_blossom);

    for (int i = 0; i < vertices_count; ++i) {
        if (!in_new_blossom[current_blossom[i]]) {
            continue;
        }
        current_blossom[i] = new_blossom;
        if (!visited[i]) {
            visited[i] = 1;
            queue[queue_size++] = i;
        }
    }
}

int find_aug_path(int vertex) {
    int index;
    for (index = 0; index < vertices_count; ++index) {
        previous[index] = -1;
        visited[index] = 0;
        current_blossom[index] = index;
    }

    queue_size = 0;
    queue_index = 0;

    queue[queue_size++] = vertex;
    visited[vertex] = 1;

    int current_vertex, connected_vertex;
    while (queue_index < queue_size) {
        current_vertex = queue[queue_index++];
        for (index = 0; index < graph[current_vertex].size(); ++index) {
            connected_vertex = graph[current_vertex][index];
            if (connected_vertex == matching[current_vertex]) {
                continue;
            }

            if (current_blossom[connected_vertex] == current_blossom[current_vertex]) {
                continue;
            }

            if (previous[connected_vertex] == -1 &&
                (matching[connected_vertex] == -1 || previous[matching[connected_vertex]] == -1)) {
                if (connected_vertex == vertex) {
                    continue;
                }

                previous[connected_vertex] = current_vertex;

                if (matching[connected_vertex] == -1) {
                    return connected_vertex;
                }

                visited[matching[connected_vertex]] = 1;
                queue[queue_size++] = matching[connected_vertex];
                continue;
            }

            if (connected_vertex != vertex &&
                (matching[connected_vertex] == -1 || previous[matching[connected_vertex]] == -1)) {
                continue;
            }

            make_new_blossom(current_vertex, connected_vertex);
        }
    }

    return -1;
}

void improve_matching() {
    int prev_vertex_matching, prev_vertex;
    for (int vertex = 0; vertex < vertices_count; ++vertex) {
        if (matching[vertex] != -1) {
            continue;
        }

        int connected_vertex = find_aug_path(vertex);

        if (connected_vertex != -1) {
            ++matching_size;
        }

        while (connected_vertex != -1) {
            prev_vertex = previous[connected_vertex];
            prev_vertex_matching = matching[prev_vertex];
            matching[connected_vertex] = prev_vertex;
            matching[prev_vertex] = connected_vertex;
            connected_vertex = prev_vertex_matching;
        }
    }
}

int main() {
    std::cin.sync_with_stdio(0);
    std::cin.tie(0);

    std::cin >> vertices_count;

    int first, second;
    while (std::cin >> first) {
        std::cin >> second;
        graph[first - 1].push_back(second - 1);
        graph[second - 1].push_back(first - 1);
    }

    for (int i = 0; i < vertices_count; ++i) {
        matching[i] = -1;
    }

    matching_size = 0;

    set_random_matching();
    improve_matching();

    std::cout << matching_size * 2 << "\n";
    for (int i = 0; i < vertices_count; ++i) {
        if (matching[i] > i) {
            std::cout << i + 1 << " " << matching[i] + 1 << "\n";
        }
    }

    return 0;
}
