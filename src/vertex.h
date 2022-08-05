#include <array>

class Vertex {
  public:
    Vertex(int x, int y, int z) : coords({x, y, z}) { }
    const std::array<int, 3> coords;
};
