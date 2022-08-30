#ifndef CONTIGUOUS_LIST_H
#define CONTIGUOUS_LIST_H

#include <list>
#include <random>
#include <vector>

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

// This next line is necessary because templates are weird
#include "contiguous_list.cpp"

#endif  // CONTIGUOUS_LIST_H
