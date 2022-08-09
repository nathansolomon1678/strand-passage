#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"

bool is_valid_edge(char edge);

inline Vertex operator+(const Vertex& vertex, const char edge);
inline char operator-(const Vertex& vertex1, const Vertex& vertex2);

#endif  // EDGE_H
