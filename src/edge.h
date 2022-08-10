#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"
#include <stdexcept>
#include <iostream>

bool is_valid_edge(char edge);

Vertex operator+(const Vertex& vertex, const char edge);
char operator-(const Vertex& vertex1, const Vertex& vertex2);

#endif  // EDGE_H
