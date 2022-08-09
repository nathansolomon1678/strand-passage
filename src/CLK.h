#ifndef CLK_H
#define CLK_H

#include "vertex.h"
#include "edge.h"

#include <iostream>
#include <vector>

class CLK {
  public:
    CLK();
    ~CLK();

    void set_z(double z);
    void set_q(double q);

    double get_z();
    double get_q();
    int get_length();
    std::string get_knot_as_string();

    void bfacf_move();
    void bfacf_moves(int num_steps);

  private:
    std::vector<Vertex> vertices;
    double z;
    double q;
};

#endif  // CLK_H
