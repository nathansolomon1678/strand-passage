#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <list>
#include <random>
#include <vector>
#include "vertices_and_edges.h"

template <typename T>
struct ForwardListNode {
  public:
    ForwardListNode(const T& value, ForwardListNode<T>* next);
    T value;
    ForwardListNode<T>* next;
};

/* For these purposes, a "contiguous list" is just a normal doubly-linked list,
 * except that it also stores a vector with the addresses of each element.
 * That vector is contiguous, so you can get a random element in constant time,
 * but it is not in the same order as the list
 */

template <typename T>
class ContiguousCircularListNode {
  public:
    ContiguousCircularListNode(const T& data, std::vector<ContiguousCircularListNode<T>*>& address_book);
    T data;
    ContiguousCircularListNode<T>* prev;
    ContiguousCircularListNode<T>* next;
    size_t index_in_address_book;

  private:
    std::vector<ContiguousCircularListNode<T>*>& address_book;
};

template <typename T>
class ContiguousCircularList {
  public:
    ContiguousCircularList();
    ContiguousCircularListNode<T>* head;

    void insert_node(const T& data, ContiguousCircularListNode<T>* prev);
    void create_initial_node(const T& data);
    void delete_node(const ContiguousCircularListNode<T>* node);
    ContiguousCircularListNode<T>* random_node() const;
    size_t size() const;

  private:
    std::vector<ContiguousCircularListNode<T>*> address_book;
};

// The iVec3_hashtable class is essentially a 3D array with chaining, which
// combines the benefits of two other data structures one might consider
// using to store information about every position in Z^3.
// A 3D array (of size width in each direction) would have very fast lookup,
// but would require every point to be in (\mathbb{Z} / width mathbb{Z})^3
// A hashtable could run quickly if width is a power of two and the hash function
// uses bitwise operators to put some information about each of the
// 3 coordinates into a single number. However, converting the output of a hash
// function into an index or memory address involves dividing by the number of
// buckets and using the remainder, and the C++ standard library forces the
// bucket count of an std::unordered_map to be a prime number, so that step
// runs slowly.
#define IVEC3_HASHTABLE_WIDTH 32
// That constant above must be a power of two, since that will allow fast
// bitwise operations to work (e.g. calculating something % 32 is much
// slower than calculating something & 31)
class iVec3_hashtable {
  public:
    ~iVec3_hashtable();

    bool contains(const ivec3& key);
    void insert(const ivec3& key);
    void erase(const ivec3& key);

  private:
    ForwardListNode<ivec3>* data[IVEC3_HASHTABLE_WIDTH]
                                [IVEC3_HASHTABLE_WIDTH]
                                [IVEC3_HASHTABLE_WIDTH];
};

// This next line is necessary because templates are weird
#include "data_structures.cpp"

#endif  // DATA_STRUCTURES_H
