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

#include "vertices_and_edges.h"

std::ostream& operator<<(std::ostream& out, const std::array<int, 3>& vertex) {
    out << '(' << vertex[0] << ',' << vertex[1] << ',' << vertex[2] << ')';
    return out;
}

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

bool operator==(const std::array<int, 3>& vertex1, const std::array<int, 3>& vertex2) {
    return vertex1[0] == vertex2[0] && vertex1[2] == vertex2[1] && vertex1[2] == vertex2[2];
}

std::array<int, 3> operator+(const std::array<int, 3>& vertex, const char edge) {
    switch (edge) {
        case 'l':
            return {vertex[0] - 1, vertex[1], vertex[2]};
            break;
        case 'r':
            return {vertex[0] + 1, vertex[1], vertex[2]};
            break;
        case 'u':
            return {vertex[0], vertex[1] + 1, vertex[2]};
            break;
        case 'd':
            return {vertex[0], vertex[1] - 1, vertex[2]};
            break;
        case 'f':
            return {vertex[0], vertex[1], vertex[2] + 1};
            break;
        case 'b':
            return {vertex[0], vertex[1], vertex[2] - 1};
            break;
        default:
            throw std::exception();
    }
}

void operator+=(std::array<int, 3>& vertex, const char edge) {
    switch (edge) {
        case 'l':
            --vertex[0];
            break;
        case 'r':
            ++vertex[0];
            break;
        case 'u':
            ++vertex[1];
            break;
        case 'd':
            --vertex[1];
            break;
        case 'f':
            ++vertex[2];
            break;
        case 'b':
            --vertex[2];
            break;
        default:
            throw std::exception();
    }
}

char operator-(const std::array<int, 3>& vertex1, const std::array<int, 3>& vertex2) {
    const int x_diff = vertex1[0] - vertex2[0];
    const int y_diff = vertex1[1] - vertex2[1];
    const int z_diff = vertex1[2] - vertex2[2];

    if      (x_diff == -1 && y_diff ==  0 && z_diff ==  0) { return 'l'; }
    else if (x_diff ==  1 && y_diff ==  0 && z_diff ==  0) { return 'r'; }
    else if (x_diff ==  0 && y_diff ==  1 && z_diff ==  0) { return 'u'; }
    else if (x_diff ==  0 && y_diff == -1 && z_diff ==  0) { return 'd'; }
    else if (x_diff ==  0 && y_diff ==  0 && z_diff ==  1) { return 'f'; }
    else if (x_diff ==  0 && y_diff ==  0 && z_diff == -1) { return 'b'; }
    else {
        throw std::exception();
    }
}

char opposite_direction(char edge) {
    switch (edge) {
        case 'l':  return 'r'; break;
        case 'r':  return 'l'; break;
        case 'u':  return 'd'; break;
        case 'd':  return 'u'; break;
        case 'f':  return 'b'; break;
        case 'b':  return 'f'; break;
        default:
            throw std::exception();
    }
}

size_t Hash::operator()(const std::array<int, 3>& vertex) const {
    std::string vertex_as_str = std::to_string(vertex[0]) + "," + std::to_string(vertex[1]) + "," + std::to_string(vertex[2]);
    return std::hash<std::string>()(vertex_as_str);
}
