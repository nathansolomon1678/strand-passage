// SEE THE HEADER FILE FOR DOCUMENTATION

// Because of weird template stuff, the header file includes this file, so
// we need to have a header guard here
#ifndef CONTIGUOUS_LIST_CPP
#define CONTIGUOUS_LIST_CPP

#include "contiguous_list.h"

// This contructor should only be used for creating the first element of a ContiguousList,
// since it doesn't set index_of_prev_node or index_of_next_node like the other constructor does
template <typename T>
Node<T>::Node(T& content, std::vector<Node<T>>& container) :
    content(content), container(container), index_of_prev_node(0), index_of_next_node(0) {
}

template <typename T>
Node<T>::Node(T& content, std::vector<Node<T>>& container, int index_of_prev_node, int current_index) :
    content(content), container(container), index_of_prev_node(index_of_prev_node) {
    index_of_next_node = container[index_of_prev_node].index_of_next_node;
    container[index_of_prev_node].index_of_next_node = current_index;
    container[index_of_next_node].index_of_prev_node = current_index;
}

template <typename T>
Node<T>& Node<T>::prev() {
    return container[index_of_prev_node];
}

template <typename T>
Node<T>& Node<T>::next() {
    return container[index_of_next_node];
}

template <typename T>
ContiguousList<T>::ContiguousList(int max_size) {
    std::vector<Node<T>> data;
    data.reserve(max_size);
}

template <typename T>
int ContiguousList<T>::size() {
    return data.size();
}

template <typename T>
void ContiguousList<T>::insert_first_node(T content) {
    data.push_back(Node<T>(content, data));
}

template <typename T>
void ContiguousList<T>::insert_node(T content, int index_of_prev_node) {
    data.push_back(Node<T>(content, data, index_of_prev_node, data.size()));
}

template <typename T>
void ContiguousList<T>::delete_nodes(std::vector<int> indices) {
    // Make sure that the nodes before and after the nodes to be removed are linked to each other
    for (int& index : indices) {
        Node<T>& element_to_remove = data[index];
        data[element_to_remove.index_of_prev_node].index_of_next_node = element_to_remove.index_of_next_node;
        data[element_to_remove.index_of_next_node].index_of_prev_node = element_to_remove.index_of_prev_node;
    }
    // Copy tail elements of the list to where the elements to be removed currently are
    for (int i = 0; i < indices.size(); ++i) {
        int current_index = indices[i];
        data[current_index].content            = data[data.size() - i - 1].content;
        data[current_index].index_of_prev_node = data[data.size() - i - 1].index_of_prev_node;
        data[current_index].index_of_next_node = data[data.size() - i - 1].index_of_next_node;
        data[data[current_index].index_of_prev_node].index_of_next_node = current_index;
        data[data[current_index].index_of_next_node].index_of_prev_node = current_index;
    }
    // Delete the nodes that have been copied to a new location
    for (int i = 0; i < indices.size(); ++i) {
        data.pop_back();
    }
    
}

#endif  // CONTIGUOUS_LIST_CPP
