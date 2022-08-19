#ifndef VERTICES_AND_EDGES_H
#define VERTICES_AND_EDGES_H

/* Throughout this repository, vertices are stored as std::array<int, 3>
 * and edges are stored as char. Here are the possible values for an edge:
 *
 * +---------------+-----------+--------------+
 * | Value of char | Direction |    Vector    |
 * +---------------+-----------+--------------+
 * | 'l'           | Left      | (-1,  0,  0) |
 * | 'r'           | Right     | (+1,  0,  0) |
 * | 'u'           | Up        | ( 0, +1,  0) |
 * | 'd'           | Down      | ( 0, -1,  0) |
 * | 'f'           | Forwards  | ( 0,  0, +1) |
 * | 'b'           | Backwards | ( 0,  0, -1) |
 * +---------------+-----------+--------------+
 */

#include <array>
#include <iostream>
#include <stdexcept>

std::ostream& operator<<(std::ostream& out, const std::array<int, 3>& vertex);

bool is_valid_edge(char edge);

std::array<int, 3> operator+(const std::array<int, 3>& coords, const char edge);

void operator+=(std::array<int, 3>& vertex, const char edge);

char operator-(const std::array<int, 3>& vertex1, const std::array<int, 3>& vertex2);

#endif  // VERTICES_AND_EDGES_H
