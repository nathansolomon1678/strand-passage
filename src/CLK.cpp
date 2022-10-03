#include "CLK.h"

ClkVertex::ClkVertex() :
    coords(ivec3({0, 0, 0})),
    index_of_prev(LATTICE_NULL_INDEX),
    index_of_next(LATTICE_NULL_INDEX),
    index_in_address_book(LATTICE_NULL_INDEX) {
}

ClkVertex::ClkVertex(const ivec3& coords) :
    coords(coords),
    index_of_prev(LATTICE_NULL_INDEX),
    index_of_next(LATTICE_NULL_INDEX),
    index_in_address_book(LATTICE_NULL_INDEX) {
}

void ClkVertex::erase() {
    coords = ivec3({0, 0, 0});
    index_of_prev = LATTICE_NULL_INDEX;
    index_of_next = LATTICE_NULL_INDEX;
    index_in_address_book = LATTICE_NULL_INDEX;
}

bool ClkVertex::is_erased() const {
    return coords == ivec3({0, 0, 0}) &&
           index_of_prev == LATTICE_NULL_INDEX &&
           index_of_next == LATTICE_NULL_INDEX &&
           index_in_address_book == LATTICE_NULL_INDEX;
}

size_t CLK::hash(const ivec3& coords) const {
    // Given coordinates of a vertex, returns the index of the first spot
    // in this.vertices where we'd try to find that point. Since this uses
    // linear probing, it might not actually be in that spot -- use the
    // CLK::find to get the vertex where it actually is
    return (coords[0] & LATTICE_WIDTH_MINUS_ONE) << LOG2_OF_LATTICE_WIDTH << LOG2_OF_LATTICE_WIDTH ^
           (coords[1] & LATTICE_WIDTH_MINUS_ONE) << LOG2_OF_LATTICE_WIDTH ^
           (coords[2] & LATTICE_WIDTH_MINUS_ONE);
}

size_t CLK::find_vertex(const ivec3& coords) const {
    size_t index = hash(coords);
    while (vertices[index].coords != coords) {
        if (vertices[index].is_erased()) {
            return LATTICE_NULL_INDEX;
        }
        ++index;
        if (index > LATTICE_BUCKET_COUNT_MINUS_ONE) {
            index &= LATTICE_BUCKET_COUNT_MINUS_ONE;
        }
    }
    return index;
}

CLK::CLK(const ivec3& first_coord) {
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertices[i] = ClkVertex(ivec3({0, 0, 0}));
        vertices_address_book[i] = LATTICE_NULL_INDEX;
    }
    index_of_first_vertex = hash(first_coord);
    vertices[index_of_first_vertex].coords = first_coord;
    vertices[index_of_first_vertex].index_of_prev = index_of_first_vertex;
    vertices[index_of_first_vertex].index_of_next = index_of_first_vertex;
}

CLK::CLK(const std::string& knot_filename) {
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertices[i] = ClkVertex(ivec3({0, 0, 0}));
        vertices_address_book[i] = LATTICE_NULL_INDEX;
    }

    std::ifstream file_stream(knot_filename);
    ivec3 current_vertex = {0, 0, 0};
    file_stream >> current_vertex[0] >> current_vertex[1] >> current_vertex[2];
    index_of_first_vertex = hash(current_vertex);
    vertices[index_of_first_vertex].coords = current_vertex;
    vertices[index_of_first_vertex].index_of_prev = index_of_first_vertex;
    vertices[index_of_first_vertex].index_of_next = index_of_first_vertex;
    vertices[index_of_first_vertex].index_in_address_book = 0;
    vertices_address_book[0] = index_of_first_vertex;
    knot_length = 1;
    while (file_stream >> current_vertex[0] >> current_vertex[1] >> current_vertex[2]) {
        insert_vertex(current_vertex, vertices_address_book[knot_length - 1]);
    }
}

void CLK::remove_vertex(size_t index) {
    ClkVertex& v = vertices[index];
    if (index_of_first_vertex == index) {
        index_of_first_vertex = v.index_of_next;
    }
    vertices[v.index_of_prev].index_of_next = v.index_of_next;
    vertices[v.index_of_next].index_of_prev = v.index_of_prev;
    --knot_length;
    vertices[vertices_address_book[knot_length]].index_in_address_book = v.index_in_address_book;
    vertices_address_book[v.index_in_address_book] = vertices_address_book[knot_length];
    vertices_address_book[knot_length] = LATTICE_NULL_INDEX;
    v.erase();
}

void CLK::insert_vertex(const ivec3& coords, size_t index_of_prev) {
    if (knot_length >= LATTICE_BUCKET_COUNT) {
        std::cerr << "Cannot fit any more vertices in this CLK. Current knot length: "
                  << knot_length << "\nLattice bucket count: " << LATTICE_BUCKET_COUNT
                  << std::endl;
        throw std::exception();
    }
    size_t index = hash(coords);
    while (!vertices[index].is_erased()) {
        ++index;
        if (index > LATTICE_BUCKET_COUNT_MINUS_ONE) {
            index &= LATTICE_BUCKET_COUNT_MINUS_ONE;
        }
    }
    vertices[index] = ClkVertex(coords);
    ClkVertex& v = vertices[index];
    vertices_address_book[knot_length] = index;
    v.index_in_address_book = knot_length;
    ++knot_length;
    v.index_of_prev = index_of_prev;
    v.index_of_next = vertices[index_of_prev].index_of_next;
    vertices[v.index_of_prev].index_of_next = index;
    vertices[v.index_of_next].index_of_prev = index;
        
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

double CLK::get_z()      const { return z; }
double CLK::get_q()      const { return q; }
size_t CLK::get_length() const { return knot_length; }

std::string CLK::get_knot_as_string() const {
    std::string compressed_knot_data = "";
    size_t current_vertex_index = index_of_first_vertex;
    for (int i = 0; i < knot_length; ++i) {
        size_t next_vertex_index = vertices[current_vertex_index].index_of_next;
        compressed_knot_data.push_back((char) (vertices[next_vertex_index].coords - vertices[current_vertex_index].coords));
        current_vertex_index = next_vertex_index;
    }
    return compressed_knot_data;
}

bool CLK::contains_vertex(const ivec3& coords) const {
    return find_vertex(coords) != LATTICE_NULL_INDEX;
}

void CLK::bfacf_move() {
    // Some edges can't move in any direction, so continue trying until one move has been performed
    while (true) {
        // Assume knot_length is tiny compared to RAND_MAX
        size_t random_vertex_index = vertices_address_book[std::rand() % knot_length];
        if (perform_move(random_vertex_index)) {
            break;
        }
    }
}

void CLK::bfacf_moves(int num_steps) {
    for (int i = 0; i < num_steps; ++i) {
        bfacf_move();
        std::cout <<get_knot_as_string() << std::endl;
    }
}

double CLK::move_probability(size_t start_vertex_index, Edge direction) const {
    // Given the first vertex of the edge to be moved and the direction to move it in,
    // returns the probability of that type of move

    // start_vertex and end_vertex are the points at the beginning and end of
    // the edge to be moved
    // TODO: combine this with the perform_move function so that these vars below
    // don't need to be redefined all the time
    size_t end_vertex_index = vertices[start_vertex_index].index_of_next;
    size_t next_vertex_index = vertices[end_vertex_index].index_of_next;
    size_t prev_vertex_index = vertices[start_vertex_index].index_of_prev;
    const ivec3& start_vertex = vertices[start_vertex_index].coords;
    const ivec3& end_vertex   = vertices[  end_vertex_index].coords;
    const ivec3& prev_vertex  = vertices[ prev_vertex_index].coords;
    const ivec3& next_vertex  = vertices[ next_vertex_index].coords;

    const Edge current_edge = end_vertex - start_vertex;
    if (current_edge == direction || current_edge == opposite_direction(direction)) {
        return 0;
    }

    if (prev_vertex - start_vertex == direction) {
        if (next_vertex - end_vertex == direction) {
            // -2 move
            if (knot_length > 4) {
                return probability_of_m2_move;
            }
        } else {
            // 0 move (swap this edge with the previous edge)
            if (!contains_vertex(end_vertex + direction)) {
                return probability_of_0_move;
            }
        }
    } else {
        if (next_vertex - end_vertex == direction) {
            // 0 move (swap this edge with the next edge)
            if (!contains_vertex(start_vertex + direction)) {
                return probability_of_0_move;
            }
        } else {
            // +2 move
            if (!contains_vertex(start_vertex + direction) &&
                !contains_vertex(end_vertex + direction)) {
                return probability_of_p2_move;
            }
        }
    }

    return 0;
}

bool CLK::perform_move(size_t start_vertex_index) {
    // Given the first vertex of the edge to be moved, determines the direction to move it in,
    // then moves, adds, and deletes vertices as necessary to perform that move
    const Edge possible_move_directions[6] = {'l', 'r', 'u', 'd', 'f', 'b'};
    double possible_move_probabilities[6] = {0, 0, 0, 0, 0, 0};
    double total_probability = 0;
    

    // start_vertex and end_vertex are the points at the beginning and end of
    // the edge to be moved
    size_t end_vertex_index = vertices[start_vertex_index].index_of_next;
    size_t next_vertex_index = vertices[end_vertex_index].index_of_next;
    size_t prev_vertex_index = vertices[start_vertex_index].index_of_prev;
    const ivec3& start_vertex = vertices[start_vertex_index].coords;
    const ivec3& end_vertex   = vertices[  end_vertex_index].coords;
    const ivec3& prev_vertex  = vertices[ prev_vertex_index].coords;
    const ivec3& next_vertex  = vertices[ next_vertex_index].coords;

    for (int i = 0; i < 6; ++i) {
        const double probability = move_probability(start_vertex_index, possible_move_directions[i]);
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

    if (prev_vertex - start_vertex == direction &&
        next_vertex - end_vertex == direction) {
        // Perform a -2 move
        remove_vertex(start_vertex_index);
        remove_vertex(end_vertex_index);
    } else if (prev_vertex - start_vertex != direction &&
               next_vertex - end_vertex != direction) {
        // Perform a +2 move
        insert_vertex(start_vertex + direction, start_vertex_index);
        insert_vertex(end_vertex + direction, vertices_address_book[knot_length - 1]);
    } else if (prev_vertex - start_vertex == direction) {
        // Perform a 0 move by swapping this edge with the previous edge
        // Note that remove_vertex can change the index of other vertices,
        // so we have to call insert_vertex before remove_vertex
        insert_vertex(end_vertex + direction, start_vertex_index);
        remove_vertex(start_vertex_index);
    } else {
        // Perform a 0 move by swapping this edge with the next edge
        insert_vertex(start_vertex + direction, start_vertex_index);
        remove_vertex(end_vertex_index);
    }

    return true;
}

// bool is_valid_CLK(std::string clk_as_str) {
//     ivec3 coords = {0, 0, 0};
//     iVec3_hashtable<ivec3*> occupied_vertices;
//     for (char letter : clk_as_str) {
//         Edge current_edge = Edge(letter);
//         coords += current_edge;
//         if (!is_valid_edge(current_edge)) { return false; }
//         if (occupied_vertices.contains(coords)) { return false; }
//         occupied_vertices.insert(coords);
//     }
//     return coords == ivec3({0, 0, 0});
// }
