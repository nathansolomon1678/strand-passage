#ifndef CLK_H
#define CLK_H

#include "contiguous_list.h"
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

    double probability_of_p2_move();
    double probability_of_m2_move();
    double probability_of_0_move();

  private:
    ContiguousCircularList<std::array<int, 3>> vertices;
    std::unordered_set<std::array<int, 3>, Hash> vertices_hashmap;
    double z;
    double q;
    double move_probability(ContiguousCircularListNode<ivec3>* node, Edge direction);
    bool perform_move(ContiguousCircularListNode<ivec3>* node);
};

bool is_valid_CLK(std::string clk_as_str);

#endif  // CLK_H
