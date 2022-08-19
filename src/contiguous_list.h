#ifndef CONTIGUOUS_LIST_H
#define CONTIGUOUS_LIST_H

#include <vector>

/* Defines an element of a ContiguousList object, which is defined below.
 * Each Node stores an object of type T, as well as the indices of the
 * previous and next Node objects within the Contiguous List
 */
template <typename T>
class Node {
  public:
    Node(T& content, std::vector<Node<T>>& container);
    Node(T& content, std::vector<Node<T>>& container, int index_of_prev_node, int current_index);

    T content;
    int index_of_prev_node;
    int index_of_next_node;
    Node<T>& prev();
    Node<T>& next();

  private:
    std::vector<Node<T>>& container;
};

/* This class is essentially a doubly linked list, except that the elements
 * are all adjacent in memory, even though their order in memory is not the
 * same as their order in the list. This is useful because then we can get
 * a random element in constant time, while still being able to traverse the
 * list in constant time. To keep the elements contiguous, we store them in
 * an std::vector (which might be resized occasionally, but that's fine), 
 * and instead of deleting elements, we swap them with the last element and
 * then pop it.
 */
template <typename T>
class ContiguousList {
  public:
    ContiguousList(int max_size = 4);
    int size();
    void insert_first_node(T content);
    void insert_node(T content, int index_of_prev_node);
    void delete_nodes(std::vector<int> indices);
    std::vector<Node<T>> data;
};

// Weird template stuff requires including the defintions in the header file
#include "contiguous_list.cpp"

#endif  // CONTIGUOUS_LIST_H
