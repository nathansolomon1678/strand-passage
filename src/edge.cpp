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

Vertex operator+(const Vertex& vertex, const char edge) {
    std::array<int, 3> coords = vertex.coords;

    switch (edge) {
        case 'l':
            --coords[0];
            break;
        case 'r':
            ++coords[0];
            break;
        case 'u':
            ++coords[1];
            break;
        case 'd':
            --coords[1];
            break;
        case 'f':
            ++coords[2];
            break;
        case 'b':
            --coords[2];
            break;
        default:
            throw std::exception();
    }

    Vertex new_vertex(coords);
    return new_vertex;
}

char operator-(const Vertex& vertex1, const Vertex& vertex2) {
    // Note: this function assumes the difference between the vertices is a valid edge
    const int x_diff = vertex1.coords[0] - vertex2.coords[0];
    const int y_diff = vertex1.coords[1] - vertex2.coords[1];
    const int z_diff = vertex1.coords[2] - vertex2.coords[2];

    if (x_diff == 1 && y_diff == 0 && z_diff == 0) {
        return 'r';
    } else if (x_diff == -1 && y_diff ==  0 && z_diff ==  0) {
        return 'l';
    } else if (x_diff ==  0 && y_diff ==  1 && z_diff ==  0) {
        return 'u';
    } else if (x_diff ==  0 && y_diff == -1 && z_diff ==  0) {
        return 'd';
    } else if (x_diff ==  0 && y_diff ==  0 && z_diff ==  1) {
        return 'f';
    } else if (x_diff ==  0 && y_diff ==  0 && z_diff == -1) {
        return 'b';
    } else {
        throw std::exception();
    }
}
