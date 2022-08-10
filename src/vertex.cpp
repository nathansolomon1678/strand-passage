#include "vertex.h"

// A CLK object stores Vertex objects in an std::vector, but that's just so that
// we can get a random one of them in constant time. That std::vector doesn't
// actually keep track of the order. Instead, each vertex object stores the index
// of the previous and next vertices, so we can use vertices together like a
// doubly-linked list.

Vertex::Vertex(int x, int y, int z) :
    // This constructure does not reorganize the doubly-linked list, so
    // before adding it to the std::vector of vertices, remember to set
    // index_of_prev and index_of_next
    coords({x, y, z}),
    index_of_prev(0),
    index_of_next(0)
{ }

Vertex::Vertex(const std::array<int, 3>& coords) :
    coords(coords),
    index_of_prev(0),
    index_of_next(0)
{ }

std::ostream& operator<<(std::ostream& out, const Vertex& v) {
    out << '(' << v.coords[0] << ',' << v.coords[1] << ',' << v.coords[2] << ')';
    return out;
}
