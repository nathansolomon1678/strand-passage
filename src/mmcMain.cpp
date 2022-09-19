#include "CLK.h"
#include <ctime>
#include <chrono>

int main(int argc, char* argv[]) {
    std::srand(std::time(NULL));

    CLK knot("initial/0_1s");
    knot.set_z(.213);
    std::cout << "z = " << knot.get_z() << std::endl;
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    knot.bfacf_moves(1000000);
    std::cout << "Time taken to perform 1 million BFACF moves: "
              << (std::chrono::system_clock::now() - start).count() / (double) 1000000000
              << " seconds" << std::endl;
    return 0;
}
