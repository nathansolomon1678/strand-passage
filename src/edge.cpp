/* +--------------+-----------+--------------+
 * | Abbreviation | Direction |    Vector    |
 * +--------------+-----------+--------------+
 * | l            | Left      | (-1,  0,  0) |
 * | r            | Right     | (+1,  0,  0) |
 * | u            | Up        | ( 0, +1,  0) |
 * | d            | Down      | ( 0, -1,  0) |
 * | f            | Forwards  | ( 0,  0, +1) |
 * | b            | Backwards | ( 0,  0, -1) |
 * +--------------+-----------+--------------+
 */

#include "edge.h"

bool is_valid_edge(char edge) {
    // Edges must be one of the 6 possible directions:
    // Left, Right, Up, Down, Forwards, Backwards
    if (edge < 'b' || edge > 'u') { return false; }
    return (
        edge == 'l' ||
        edge == 'r' ||
        edge == 'u' ||
        edge == 'd' ||
        edge == 'f' ||
        edge == 'b'
    );
}

inline Vertex operator+(const Vertex& vertex, const char edge) {
    Vertex new_vertex(vertex.coords);

    switch (edge) {
        case 'l':
            --new_vertex.coords[0];
            break;
        case 'r':
            ++new_vertex.coords[0];
            break;
        case 'u':
            ++new_vertex.coords[1];
            break;
        case 'd':
            --new_vertex.coords[1];
            break;
        case 'f':
            ++new_vertex.coords[2];
            break;
        case 'b':
            --new_vertex.coords[2];
            break;
    }

    return new_vertex
}

inline char operator-(const Vertex& vertex1, const Vertex& vertex2) {
    // Note: this function assumes the difference between the vertices is a valid edge
    if (vertex1.coords[0] != vertex2.coords[0]) {
        if (vertex1.coords[0] > vertex2.coords[0]) {
            return 'r';
        } else {
            return 'l';
        }
    } else if (vertex1.coords[1] != vertex2.coords[1]) {
        if (vertex1.coords[1] > vertex2.coords[1]) {
            return 'u';
        } else {
            return 'd';
        }
    }
    if (vertex1.coords[2] > vertex2.coords[2]) {
        return 'f';
    }
    return 'b';
}
