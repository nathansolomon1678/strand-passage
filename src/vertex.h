#ifndef VERTEX_H
#define VERTEX_H

#include <array>
#include <iostream>

class Vertex {
  public:
    Vertex(int x, int y, int z);
    Vertex(const std::array<int, 3>& coords);

    const std::array<int, 3> coords;
    int index_of_prev;
    int index_of_next;
};

std::ostream& operator<<(std::ostream& out, const Vertex& v);

#endif  // VERTEX_H
