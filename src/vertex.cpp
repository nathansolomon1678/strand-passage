#include "vertex.h"

Vertex::Vertex(int x, int y, int z) :
    coords({x, y, z}),
    prev(this),
    next(this)
{ }

Vertex::~Vertex() { }
