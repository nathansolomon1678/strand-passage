#ifndef CLK_H
#define CLK_H

#include "vertices_and_edges.h"

#include <array>
#include <fstream>
#include <iostream>
#include <unordered_set>

/* The CLK class stores vertices in a flattened 3D array, which uses a hash
 * function and linear probing to find an element (given its coordinates) in
 * constant time. For that process to run as quickly as possible, it uses
 * bitwise operations with the constants defined below.
 */
#define LOG2_OF_LATTICE_WIDTH          5
#define LATTICE_WIDTH                  (1 << LOG2_OF_LATTICE_WIDTH)
#define LATTICE_BUCKET_COUNT           (LATTICE_WIDTH * LATTICE_WIDTH * LATTICE_WIDTH)
#define LATTICE_WIDTH_MINUS_ONE        (LATTICE_WIDTH - 1)
#define LATTICE_BUCKET_COUNT_MINUS_ONE (LATTICE_BUCKET_COUNT - 1)
#define LATTICE_NULL_INDEX             (LATTICE_BUCKET_COUNT)

struct ClkVertex {
  public:
    ClkVertex();
    ClkVertex(const ivec3& coords);

    ivec3 coords;
    size_t index_of_prev;
    size_t index_of_next;
    size_t index_in_address_book;

    void erase();
    bool is_erased() const;
};

class CLK {
  public:
    CLK(const ivec3& first_coord);
    CLK(const std::string& knot_filename);
    size_t get_length() const;
    std::string get_knot_as_string() const;
    void print_coords() const;
    bool contains_vertex(const ivec3& coords) const;

    void set_z(double z);
    void set_q(double q);
    double get_z() const;
    double get_q() const;
    void bfacf_move();
    void bfacf_moves(int num_steps);

  private:
    std::array<ClkVertex, LATTICE_BUCKET_COUNT> vertices;
    std::array<size_t, LATTICE_BUCKET_COUNT> vertices_address_book;
    size_t knot_length;
    size_t index_of_first_vertex;
    size_t hash(const ivec3& coords) const;
    size_t find_vertex(const ivec3& coords) const;
    void remove_vertex(size_t index);
    void insert_vertex(const ivec3& coords, size_t index_of_prev);

    double z;
    double q;
    double move_probability(size_t start_vertex_index, Edge direction) const;
    bool perform_move(size_t start_vertex_index);
    void set_move_probabilities();
    double probability_of_p2_move;
    double probability_of_m2_move;
    double probability_of_0_move;
};

bool is_valid_CLK(std::string clk_as_str);
//TODO: define function to initialize a CLK from a string or from a file (of various file types)

#endif  // CLK_H
