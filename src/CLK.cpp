#include "CLK.h"

CLK::CLK(std::string filename) {
    // Constructs a CLK from a knot in Rob Scharein's CUBE file format for knots
    std::ifstream file_stream(filename);
    int x, y, z;

    // ContiguousList must be initialized with at least two elements before
    // inserting elements normally, or else the linking won't work
    file_stream >> x >> y >> z;
    vertices.create_initial_node({x, y, z});
    vertices_hashmap.insert({x, y, z});

    while (file_stream >> x >> y >> z) {
        vertices.insert_node({x, y, z}, vertices.head->prev);
        vertices_hashmap.insert({x, y, z});
    }
}

void CLK::set_move_probabilities() {
    // TODO: account for the Q parameter when determining move probabilities
    // (and find literature to justify the formula for the probabilities)
    // Also, if it uses the Q parameter, precompute move proabilites for
    // several possible lengths
    const double z_squared = z * z;
    probability_of_p2_move = z_squared / (1 + 3 * z_squared);
    probability_of_m2_move = 1 / (1 + 3 * z * z);
    probability_of_0_move = (1 + z_squared) / 2 / (1 + 3 * z_squared);
}

void CLK::set_z(double z) {
    this->z = z;
    set_move_probabilities();
}

void CLK::set_q(double q) {
    this->q = q;
    set_move_probabilities();
}

double CLK::get_z() { return z; }
double CLK::get_q() { return q; }
int CLK::length() { return vertices.size(); }

std::string CLK::get_knot_as_string() {
    std::string compressed_knot_data = "";
    ContiguousCircularListNode<ivec3>* current_node = vertices.head;
    for (int i = 0; i < vertices.size(); ++i) {
        compressed_knot_data.push_back((char) (current_node->next->data - current_node->data));
        current_node = current_node->next;
    }
    return compressed_knot_data;
}

bool CLK::contains_vertex(const ivec3& coords) {
    return vertices_hashmap.find(coords) != vertices_hashmap.end();
}

void CLK::bfacf_move() {
    // Some edges can't move in any direction, so continue trying until one move has been performed
    while (true) {
        if (perform_move(vertices.random_node())) {
            break;
        }
    }
}

void CLK::bfacf_moves(int num_steps) {
    for (int i = 0; i < num_steps; ++i) {
        bfacf_move();
    }
}

double CLK::move_probability(ContiguousCircularListNode<ivec3>* node, Edge direction) {
    // Given the first vertex of the edge to be moved (`node`) and the direction to move it in,
    // returns the probability of that type of move

    const Edge current_edge = node->next->data - node->data;
    if (current_edge == direction || current_edge == opposite_direction(direction)) {
        return 0;
    }

    if (node->prev->data - node->data == direction) {
        if (node->next->next->data - node->next->data == direction) {
            // -2 move
            if (vertices.size() > 4) {
                return probability_of_m2_move;
            }
        } else {
            // 0 move (swap this edge with the previous edge)
            if (!contains_vertex(node->next->data + direction)) {
                return probability_of_0_move;
            }
        }
    } else {
        if (node->next->next->data - node->next->data == direction) {
            // 0 move (swap this edge with the next edge)
            if (!contains_vertex(node->data + direction)) {
                return probability_of_0_move;
            }
        } else {
            // +2 move
            if (!contains_vertex(node->data + direction) &&
                !contains_vertex(node->next->data + direction)) {
                return probability_of_p2_move;
            }
        }
    }

    return 0;
}

bool CLK::perform_move(ContiguousCircularListNode<ivec3>* node) {
    // Given the first vertex of the edge to be moved, determines the direction to move it in,
    // then moves, adds, and deletes vertices as necessary to perform that move
    const Edge possible_move_directions[6] = {'l', 'r', 'u', 'd', 'f', 'b'};
    double possible_move_probabilities[6] = {0, 0, 0, 0, 0, 0};
    double total_probability = 0;

    for (int i = 0; i < 6; ++i) {
        const double probability = move_probability(node, possible_move_directions[i]);
        possible_move_probabilities[i] = probability;
        total_probability += probability;
    }

    // Select one of the 6 possible directions at random using the probabilities calculated above
    double random_num = (double) std::rand() / (double) RAND_MAX;
    if (random_num > total_probability) {
        return false;
    }
    Edge direction;
    for (int i = 0; i < 6; ++i) {
        if (random_num <= possible_move_probabilities[i]) {
            direction = possible_move_directions[i];
            break;
        }
        random_num -= possible_move_probabilities[i];
    }

    if (node->prev->data - node->data == direction &&
        node->next->next->data - node->next->data == direction) {
        // Perform a -2 move
        vertices_hashmap.erase(vertices_hashmap.find(node->data));
        vertices_hashmap.erase(vertices_hashmap.find(node->next->data));
        vertices.delete_node(node->next);
        vertices.delete_node(node);
    } else if (node->prev->data - node->data != direction &&
               node->next->next->data - node->next->data != direction) {
        // Perform a +2 move
        ivec3 new_vertex_coords = node->next->data + direction;
        vertices_hashmap.insert(new_vertex_coords);
        vertices.insert_node(new_vertex_coords, node);
        new_vertex_coords = node->data + direction;
        vertices_hashmap.insert(new_vertex_coords);
        vertices.insert_node(new_vertex_coords, node);
    } else if (node->prev->data - node->data == direction) {
        // Perform a 0 move by swapping this edge with the previous edge
        vertices_hashmap.erase(vertices_hashmap.find(node->data));
        node->data = node->next->data + direction;
        vertices_hashmap.insert(node->data);
    } else {
        // Perform a 0 move by swapping this edge with the next edge
        vertices_hashmap.erase(vertices_hashmap.find(node->next->data));
        node->next->data = node->data + direction;
        vertices_hashmap.insert(node->next->data);
    }

    return true;
}

bool is_valid_CLK(std::string clk_as_str) {
    ivec3 coords = {0, 0, 0};
    std::unordered_set<ivec3, Hash> occupied_vertices;
    for (char letter : clk_as_str) {
        Edge current_edge = Edge(letter);
        coords += current_edge;
        if (!is_valid_edge(current_edge)) { return false; }
        if (occupied_vertices.find(coords) != occupied_vertices.end()) { return false; }
        occupied_vertices.insert(coords);
    }
    return coords == ivec3({0, 0, 0});
}
