#include "CLK.h"
#include <ctime>

int main(int argc, char* argv[]) {
    std::srand(std::time(NULL));

    CLK knot("initial/0_1s");
    knot.bfacf_moves(100);
    return 0;
}
