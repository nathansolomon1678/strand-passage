#include "CLK.h"

CLK::CLK(std::string filename) {
    // Constructs a CLK from a knot in Rob Scharein's CUBE file format for knots
    std::ifstream file_stream(filename);
    int x, y, z;

    // ContiguousList must be initialized with at least two elements before
    // inserting elements normally, or else the linking won't work
    file_stream >> x >> y >> z;
    std::array<int, 3> first_vertex = {x, y, z};
    file_stream >> x >> y >> z;
    std::array<int, 3> second_vertex = {x, y, z};
    vertices.insert_first_nodes({first_vertex, second_vertex});
    vertices_hashmap.insert(first_vertex);
    vertices_hashmap.insert(second_vertex);

    while (file_stream >> x >> y >> z) {
        vertices.insert_node({x, y, z}, vertices.size() - 1);
        vertices_hashmap.insert({x, y, z});
    }
}

void CLK::set_z(double z) {
    this->z = z;
}

void CLK::set_q(double q) {
    this->q = q;
}

double CLK::get_z() { return z; }
double CLK::get_q() { return q; }
int CLK::length() { return vertices.size(); }

std::string CLK::get_knot_as_string() {
    std::string compressed_knot_data = "";
    int current_index = 0;
    for (int i = 0; i < vertices.size(); ++i) {
        int next_index = vertices.data[current_index].index_of_next_node;

        compressed_knot_data.push_back(vertices.data[next_index].content - vertices.data[current_index].content);
        current_index = next_index;
    }
    if (!is_valid_CLK(compressed_knot_data)) {
        std::cout << "Invalid CLK: \"" << compressed_knot_data << '"' << std::endl;
        std::cout << "Vertices occupied (not in order): " << std::endl;
        for (const std::array<int, 3>& coords : vertices_hashmap) {
            std::cout << coords << std::endl;
        }
        throw std::exception();
    }
    return compressed_knot_data;
}

void CLK::print_coords() {
    int current_index = 0;
    for (int i = 0; i < vertices.size(); ++i) {
        std::array<int, 3> coords = vertices.data[current_index].content;
        std::cout << contains_vertex(coords) << '\t' << coords << std::endl;
        current_index = vertices.data[current_index].index_of_next_node;
    }
}

bool CLK::contains_vertex(std::array<int, 3> coords) {
    return vertices_hashmap.find(coords) != vertices_hashmap.end();
}

void CLK::bfacf_move() {
    // Some edges can't move in any direction, so continue trying until one move has been performed
    while (true) {
        // The BFACF algorithm begins by selecting a random edge
        // vertex_index is the index of the vertex at the beginning of that edge
        int vertex_index = std::rand() % vertices.size();
        if (perform_move(vertex_index)) {
            break;
        }
    }
}

void CLK::bfacf_moves(int num_steps) {
    for (int i = 0; i < num_steps; ++i) {
        print_coords();
        std::cout << get_knot_as_string() << std::endl;
        bfacf_move();
    }
}

// TODO: account for the Q parameter when determining move probabilities (and find literature to justify the formula for the probabilities)
double CLK::probability_of_p2_move() {
    // Probability of a +2 move
    const double z_squared = z * z;
    return z_squared / (1 + 3 * z_squared);
}
double CLK::probability_of_m2_move() {
    // Probability of a -2 move
    return 1 / (1 + 3 * z * z);
}
double CLK::probability_of_0_move() {
    // Probability of a 0 move (doesn't change length of knot)
    const double z_squared = z * z;
    return (1 + z_squared) / 2 / (1 + 3 * z_squared);
}

double CLK::move_probability(int vertex_index, char direction) {
    // Given the first vertex of the edge to be moved and the direction to move it in,
    // returns the probability of that type of move
    Node<std::array<int, 3>>& start_vertex        = vertices.data[vertex_index];
    Node<std::array<int, 3>>& end_vertex          = start_vertex.next();
    Node<std::array<int, 3>>& vertex_before_start = start_vertex.prev();
    Node<std::array<int, 3>>& vertex_after_end    = end_vertex.next();

    // If the edge is parallel or antiparallel to the move direction, return 0
    const char current_edge = end_vertex.content - start_vertex.content;
    if (current_edge == direction || current_edge == opposite_direction(direction)) {
        return 0;
    }

    if (vertex_before_start.content - start_vertex.content == direction &&
        vertex_after_end.content    - end_vertex.content   == direction) {
        // -2 move
        if (vertices.size() > 4) {
            return probability_of_m2_move();
        }
    } else if (vertex_before_start.content - start_vertex.content != direction &&
               vertex_after_end.content    - end_vertex.content   != direction) {
        // +2 move
        if (!contains_vertex(start_vertex.content + direction) &&
            !contains_vertex(  end_vertex.content + direction)) {
            return probability_of_p2_move();
        }
    } else if (vertex_before_start.content - start_vertex.content == direction) {
        // 0 move (swap this edge with the previous edge)
        if (!contains_vertex(end_vertex.content + direction)) {
            return probability_of_0_move();
        }
    } else {
        // 0 move (swap this edge with the next edge)
        if (!contains_vertex(start_vertex.content + direction)) {
            return probability_of_0_move();
        }
    }
    return 0;
}

bool CLK::perform_move(int vertex_index) {
    // Given the first vertex of the edge to be moved, determines the direction to move it in,
    // then moves, adds, and deletes vertices as necessary to perform that move
    char possible_move_directions[6] = {'l', 'r', 'u', 'd', 'f', 'b'};
    double possible_move_probabilities[6] = {0, 0, 0, 0, 0, 0};
    double total_probability = 0;

    for (int i = 0; i < 6; ++i) {
        const double probability = move_probability(vertex_index, possible_move_directions[i]);
        possible_move_probabilities[i] = probability;
        total_probability += probability;
    }
    
    // Ensure that the probabilities of all possible moves add up to 1
    if (total_probability == 0) {
        return false;
    } else if (total_probability > 1) {
        throw std::exception();
    } else if (total_probability < 1) {
        for (int i = 0; i < 6; ++i) {
            possible_move_probabilities[i] /= total_probability;
        }
    }

    // Select one of the 6 possible directions at random using the probabilities calculated above
    double random_num = (double) std::rand() / (double) RAND_MAX;
    char direction;
    for (int i = 0; i < 6; ++i) {
        if (random_num <= possible_move_probabilities[i]) {
            direction = possible_move_directions[i];
            break;
        }
        random_num -= possible_move_probabilities[i];
    }

    // TODO: clean this up, wo it doesn't use so many references to node objects and so it doesn't have to use `.content` a gazillion times
    // The edge to be moved is the one between start_vertex.content and end_vertex.content
    Node<std::array<int, 3>>& start_vertex        = vertices.data[vertex_index];
    Node<std::array<int, 3>>& end_vertex          = start_vertex.next();
    Node<std::array<int, 3>>& vertex_before_start = start_vertex.prev();
    Node<std::array<int, 3>>& vertex_after_end    = end_vertex.next();

    if (vertex_before_start.content - start_vertex.content == direction &&
        vertex_after_end.content    - end_vertex.content == direction) {
        // Perform a -2 move
        vertices_hashmap.erase(vertices_hashmap.find(start_vertex.content));
        vertices_hashmap.erase(vertices_hashmap.find(end_vertex.content));
        vertices.delete_nodes({vertex_index, start_vertex.index_of_next_node});
    } else if (vertex_before_start.content - start_vertex.content != direction &&
               vertex_after_end.content    - end_vertex.content != direction) {
        // Perform a +2 move
        std::array<int, 3> new_vertex_coords = end_vertex.content + direction;
        vertices_hashmap.insert(new_vertex_coords);
        vertices.insert_node(new_vertex_coords, vertex_index);

        new_vertex_coords = vertices.data[vertex_index].content + direction;
        vertices_hashmap.insert(new_vertex_coords);
        vertices.insert_node(new_vertex_coords, vertex_index);
    } else if (vertex_before_start.content - start_vertex.content == direction) {
        // Perform a 0 move by swapping this edge with the previous edge
        vertices_hashmap.erase(vertices_hashmap.find(start_vertex.content));
        start_vertex.content = end_vertex.content + direction;
        vertices_hashmap.insert(start_vertex.content);
    } else {
        // Perform a 0 move by swapping this edge with the next edge
        vertices_hashmap.erase(vertices_hashmap.find(end_vertex.content));
        end_vertex.content = start_vertex.content + direction;
        vertices_hashmap.insert(end_vertex.content);
    }

    std::cout << std::endl;
    return true;
}

bool is_valid_CLK(std::string clk_as_str) {
    // TODO: use an unordered set to check that this path is self avoiding
    std::array<int, 3> coords = {0, 0, 0};
    char last_edge = clk_as_str[clk_as_str.size() - 1];
    for (char edge : clk_as_str) {
        if (!is_valid_edge(edge)) { return false; }
        if (edge == opposite_direction(last_edge)) { return false; }
        coords += edge;
        last_edge = edge;
    }
    return operator==(coords, {0, 0, 0});
}
