#include "CLK.h"

CLK::CLK(std::string filename) {
    // Constructs a CLK from a knot in Rob Scharein's CUBE file format for knots
    std::ifstream file_stream(filename);
    int x, y, z;

    file_stream >> x >> y >> z;
    vertices.insert_first_node({x, y, z});

    while (file_stream >> x >> y >> z) {
        vertices.insert_node({x, y, z}, vertices.size() - 1);
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
        throw std::exception();
    }
    return compressed_knot_data;
}

double CLK::move_probability(int vertex_index, char move_direction) {
    // Given the first vertex of the edge to be moved and the direction to move it in,
    // returns the probability of that type of move
    return .15;  // PLACEHOLDER
}

void CLK::perform_move(int vertex_index) {
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
        return;
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
        }
        random_num -= possible_move_probabilities[i];
    }

    // TODO: use a hash map to check if the locations that the vertices are moving into are taken

    // The edge to be moved is the one between start_vertex.content and end_vertex.content
    Node<std::array<int, 3>>& start_vertex        = vertices.data[vertex_index];
    Node<std::array<int, 3>>& end_vertex          = start_vertex.next();
    Node<std::array<int, 3>>& vertex_before_start = start_vertex.prev();
    Node<std::array<int, 3>>& vertex_after_end    = end_vertex.next();

    if (vertex_before_start.content - start_vertex.content == direction &&
        vertex_after_end.content - end_vertex.content == direction) {
        // Perform a -2 move
        vertices.delete_nodes({vertex_index, start_vertex.index_of_next_node});
    } else if (vertex_before_start.content - start_vertex.content != direction &&
               vertex_after_end.content - end_vertex.content != direction) {
        // Perform a +2 move
        vertices.insert_node(start_vertex.content + direction, vertex_index);
        vertices.insert_node(end_vertex.content + direction, start_vertex.index_of_next_node);
    } else if (vertex_before_start.content - start_vertex.content == direction) {
        // Perform a 0 move by swapping this edge with the previous edge
        start_vertex.content = end_vertex.content + direction;
    } else {
        // Perform a 0 move by swapping this edge with the next edge
        end_vertex.content = start_vertex.content + direction;
    }

    std::cout << get_knot_as_string() << std::endl;
}

void CLK::bfacf_move() {
    // The BFACF algorithm begins by selecting a random edge
    // vertex_index is the index of the vertex at the beginning of that edge
    int vertex_index = std::rand() % vertices.size();
    perform_move(vertex_index);
}

void CLK::bfacf_moves(int num_steps) {
    for (int i = 0; i < num_steps; ++i) {
        this->bfacf_move();
    }
}

bool is_valid_CLK(std::string clk_as_str) {
    std::array<int, 3> coords = {0, 0, 0};
    char last_edge = clk_as_str[clk_as_str.size() - 1];
    for (char edge : clk_as_str) {
        if (!is_valid_edge(edge)) { return false; }
        if (edge == opposite_direction(last_edge)) { return false; }
        coords += edge;
        last_edge = edge;
    }
    if (coords[0] != 0) { return false; }
    if (coords[1] != 0) { return false; }
    if (coords[2] != 0) { return false; }
    return true;
}
