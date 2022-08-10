#include "CLK.h"

CLK::CLK(std::string filename) {
    // Constructs a CLK from a knot in Rob Scharein's CUBE file format for knots
    std::ifstream file_stream(filename);
    int x, y, z;
    while (file_stream >> x >> y >> z) {
        vertices.emplace_back(x, y, z);
    }

    for (int i = 1; i < vertices.size() - 1; ++i) {
        vertices[i].index_of_prev = i - 1;
        vertices[i].index_of_next = i + 1;
    }
    vertices[0].index_of_prev = vertices.size();
    vertices[0].index_of_next = 1;
    vertices[vertices.size()].index_of_prev = vertices.size() - 1;
    vertices[vertices.size()].index_of_next = 0;
}

CLK::~CLK() { }

void CLK::set_z(double z) {
    this->z = z;
}

void CLK::set_q(double q) {
    this->q = q;
    // If q is not the default value, then precompute z values for lengths between 4 and 400
}

double CLK::get_z() { return z; }
double CLK::get_q() { return q; }
int CLK::get_length() { return vertices.size(); }

std::string CLK::get_knot_as_string() {
    std::string compressed_knot_data = "";
    int current_index = 0;
    for (int i = 0; i < vertices.size(); ++i) {
        int next_index = vertices[current_index].index_of_next;

        compressed_knot_data.push_back(vertices[next_index] - vertices[current_index]);
        current_index = next_index;
    }
    return compressed_knot_data;
}

void CLK::bfacf_move() {
    // TODO: for the sake of debugging, maybe keep a pointer to the "first"
    // vertex of the knot? It might be easier to read the compressed_knot_data strings
    // if they always start at the same vertex
    std::cout << get_knot_as_string() << std::endl;
}

void CLK::bfacf_moves(int num_steps) {
    for (int i = 0; i < num_steps; ++i) {
        this->bfacf_move();
    }
}
