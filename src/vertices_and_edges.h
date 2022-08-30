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

typedef std::array<int, 3> ivec3;
typedef char Edge;
bool is_valid_edge(Edge e);

std::ostream& operator<<(std::ostream& out, const ivec3& vertex);
bool operator==(const ivec3& vertex1, const ivec3& vertex2);
ivec3 operator+(const ivec3& coords, const Edge e);
void operator+=(ivec3& vertex, const Edge e);
Edge operator-(const ivec3& vertex1, const ivec3& vertex2);
Edge opposite_direction(Edge e);

struct Hash {
    size_t operator()(const ivec3& vertex) const;
};

#endif  // VERTICES_AND_EDGES_H
