#ifndef VERTEX_H
#define VERTEX_H

#include <array>

class Vertex {
  public:
    Vertex(int x, int y, int z);
    ~Vertex();

    const std::array<int, 3> coords;
    Vertex* prev;
    Vertex* next;
};

#endif  // VERTEX_H
