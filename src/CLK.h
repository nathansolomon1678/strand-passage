#ifndef CLK_H
#define CLK_H

#include "data_structures.h"
#include "vertices_and_edges.h"

#include <array>
#include <fstream>
#include <iostream>
#include <unordered_set>

class CLK {
  public:
    CLK(std::string filename);

    void set_z(double z);
    void set_q(double q);

    double get_z();
    double get_q();
    int length();
    std::string get_knot_as_string();
    void print_coords();
    bool contains_vertex(const ivec3& coords);

    void bfacf_move();
    void bfacf_moves(int num_steps);

  private:
    ContiguousCircularList<ivec3> vertices;
    // TODO: instead of a hashtable of vertices, use a hashmap from vertices to their addresses
    // in memory, so that strand passage can run in constant time
    iVec3_hashtable vertices_hashtable;
    double z;
    double q;
    double move_probability(ContiguousCircularListNode<ivec3>* node, Edge direction);
    bool perform_move(ContiguousCircularListNode<ivec3>* node);

    void set_move_probabilities();
    double probability_of_p2_move;
    double probability_of_m2_move;
    double probability_of_0_move;
};

bool is_valid_CLK(std::string clk_as_str);

#endif  // CLK_H
