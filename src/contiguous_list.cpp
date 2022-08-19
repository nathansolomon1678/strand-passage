// SEE THE HEADER FILE FOR DOCUMENTATION
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
ContiguousList<T>::ContiguousList(T first_element, int max_size) {
    std::vector<Node<T>> data(max_size);
    data[0] = Node<T>(first_element, *this);
}

template <typename T>
int ContiguousList<T>::size() {
    return data.size();
}

template <typename T>
void ContiguousList<T>::insert_node(T content, int index_of_prev_node) {
    data.push_back(Node<T>(content, *this, index_of_prev_node, data.size()));
}

template <typename T>
void ContiguousList<T>::delete_node(int index) {
    // Make sure that the previous and next elements are linked
    Node<T>& element_to_remove = data[index];
    data[element_to_remove.index_of_prev_node].index_of_next_node = element_to_remove.index_of_next_node;
    data[element_to_remove.index_of_next_node].index_of_prev_node = element_to_remove.index_of_prev_node;
    // Copy the last element of the list to here
    data[index] = data[data.size() - 1];
    data[data[index].index_of_prev_node].index_of_next_node = index;
    data[data[index].index_of_next_node].index_of_prev_node = index;
    // Delete the last element
    data.pop_back();
}
