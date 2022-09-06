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

std::ostream& operator<<(std::ostream& out, const ivec3& vertex) {
    out << '(' << vertex[0] << ',' << vertex[1] << ',' << vertex[2] << ')';
    return out;
}

bool is_valid_edge(Edge e) {
    // Edges must be one of the 6 possible directions:
    // Left, Right, Up, Down, Forwards, Backwards
    if (e < 'b' || e > 'u') { return false; }
    return (
        e == 'l' ||
        e == 'r' ||
        e == 'u' ||
        e == 'd' ||
        e == 'f' ||
        e == 'b'
    );
}

bool operator==(const ivec3& vertex1, const ivec3& vertex2) {
    return vertex1[0] == vertex2[0] && vertex1[1] == vertex2[1] && vertex1[2] == vertex2[2];
}

ivec3 operator+(const ivec3& vertex, const Edge e) {
    switch (e) {
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
            std::cerr << "Invalid edge: " << e << std::endl;
            throw std::exception();
    }
}

void operator+=(ivec3& vertex, const Edge e) {
    switch (e) {
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
            std::cerr << "Invalid edge: " << e << std::endl;
            throw std::exception();
    }
}

Edge operator-(const ivec3& vertex1, const ivec3& vertex2) {
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
        std::cerr << "Error: vertices " << vertex1 << " and " << vertex2 << " are not adjacent." << std::endl;
        throw std::exception();
    }
}

Edge opposite_direction(Edge e) {
    switch (e) {
        case 'l':  return 'r'; break;
        case 'r':  return 'l'; break;
        case 'u':  return 'd'; break;
        case 'd':  return 'u'; break;
        case 'f':  return 'b'; break;
        case 'b':  return 'f'; break;
        default:
            std::cerr << "Invalid edge: " << e << std::endl;
            throw std::exception();
    }
}

size_t Hash::operator()(const ivec3& vertex) const {
    // Note: this is NOT guaranteed to be unique
    return (vertex[0] % 1024) << 20 + (vertex[1] % 1024) << 10 + (vertex[2] % 1024);
}
