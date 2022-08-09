#include "CLK.h"

CLK::CLK() { }

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

}

void CLK::bfacf_move() {
    std::cout << "Performing BFACF move!" << std::endl;
}

void CLK::bfacf_moves(int num_steps) {
    for (int i = 0; i < num_steps; ++i) {
        this->bfacf_move();
    }
}
