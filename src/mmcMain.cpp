#include "CLK.h"
#include <ctime>

int main(int argc, char* argv[]) {
    std::srand(std::time(NULL));

    CLK knot("initial/0_1s");
    knot.set_z(.2);
    knot.bfacf_moves(10000);
    return 0;
}
