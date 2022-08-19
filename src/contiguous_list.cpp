// SEE THE HEADER FILE FOR DOCUMENTATION

// Because of weird template stuff, the header file includes this file, so
// we need to have a header guard here
#ifndef CONTIGUOUS_LIST_CPP
#define CONTIGUOUS_LIST_CPP

#include "contiguous_list.h"

// This contructor should only be used for creating the first element of a ContiguousList,
// since it doesn't set index_of_prev_node or index_of_next_node like the other constructor does
template <typename T>
Node<T>::Node(const T& content, std::vector<Node<T>>& container) :
    content(content), container(container), index_of_prev_node(0), index_of_next_node(0) {
}

template <typename T>
Node<T>::Node(const T& content, std::vector<Node<T>>& container, int index_of_prev_node, int current_index) :
    content(content), container(container), index_of_prev_node(index_of_prev_node) {
    index_of_next_node = prev().index_of_next_node;
    prev().index_of_next_node = current_index;
    next().index_of_prev_node = current_index;
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
void ContiguousList<T>::insert_first_nodes(const std::vector<T>& elements) {
    if (elements.size() < 2) {
        throw std::exception();
    }
    for (int i = 0; i < elements.size(); ++i) {
        data.emplace_back(elements[i], data);
    }
    data[0].index_of_prev_node = elements.size() - 1;
    data[0].index_of_next_node = 1;
    for (int i = 0; i < elements.size() - 1; ++i) {
        data[i].index_of_prev_node = i - 1;
        data[i].index_of_next_node = i + 1;
    }
    data[elements.size() - 1].index_of_prev_node = elements.size() - 2;
    data[elements.size() - 1].index_of_next_node = 0;
}

template <typename T>
void ContiguousList<T>::insert_node(const T& content, int index_of_prev_node) {
    data.emplace_back(content, data, index_of_prev_node, data.size());
}

// WARNING: the delete_nodes function will move around Nodes other than
// just the nodes being deleted
template <typename T>
void ContiguousList<T>::delete_nodes(std::vector<int> indices) {
    for (int i = 0; i < indices.size(); ++i) {
        int index = indices[i];
        // Make sure that the nodes before and after the nodes to be removed are linked to each other
        Node<T>& element_to_remove = data[index];
        element_to_remove.prev().index_of_next_node = element_to_remove.index_of_next_node;
        element_to_remove.next().index_of_prev_node = element_to_remove.index_of_prev_node;
    }
    for (int i = 0; i < indices.size(); ++i) {
        int index = indices[i];
        if (index >= data.size() - indices.size()) {
            continue;
        }
        Node<T>& element_to_remove = data[index];
        // Copy tail elements of the list to where the elements to be removed currently are
        element_to_remove.content            = data[data.size() - i - 1].content;
        element_to_remove.index_of_prev_node = data[data.size() - i - 1].index_of_prev_node;
        element_to_remove.index_of_next_node = data[data.size() - i - 1].index_of_next_node;
        if (element_to_remove.index_of_prev_node >= data.size() - indices.size()) {
            element_to_remove.index_of_prev_node = indices[data.size() - element_to_remove.index_of_prev_node - 1];
        }
        if (element_to_remove.index_of_next_node >= data.size() - indices.size()) {
            element_to_remove.index_of_next_node = indices[data.size() - element_to_remove.index_of_next_node - 1];
        }
        element_to_remove.prev().index_of_next_node = index;
        element_to_remove.next().index_of_prev_node = index;
    }
    // Delete the nodes that have been copied to a new location
    for (int i = 0; i < indices.size(); ++i) {
        const int index = data.size() - i - 1;
        data.pop_back();
    }
}

#endif  // CONTIGUOUS_LIST_CPP
