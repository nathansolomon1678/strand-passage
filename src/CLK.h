#ifndef CLK_H
#define CLK_H

#include "contiguous_list.h"
#include "vertices_and_edges.h"

#include <array>
#include <fstream>
#include <iostream>

class CLK {
  public:
    CLK(std::string filename);

    void set_z(double z);
    void set_q(double q);

    double get_z();
    double get_q();
    int length();
    std::string get_knot_as_string();

    double move_probability(int vertex_index, char move_direction);
    void perform_move(int vertex_index);
    void bfacf_move();
    void bfacf_moves(int num_steps);

  private:
    ContiguousList<std::array<int, 3>> vertices;
    double z;
    double q;
};

bool is_valid_CLK(std::string clk_as_str);

#endif  // CLK_H
