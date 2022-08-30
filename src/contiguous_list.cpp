// Typically .cpp files don't need header guards, but since there's weird
// template stuff, the corresponding header file is including this file
#ifndef CONTIGUOUS_LIST_CPP
#define CONTIGUOUS_LIST_CPP

#include "contiguous_list.h"

/* See header file for documentation */

template <typename T>
ContiguousCircularListNode<T>::ContiguousCircularListNode(const T& data, std::vector<ContiguousCircularListNode<T>*>& address_book) :
    data(data),
    address_book(address_book),
    index_in_address_book(address_book.size()) {
    address_book.push_back(this);
}

template <typename T>
ContiguousCircularList<T>::ContiguousCircularList() :
    address_book({}),
    head(nullptr) {
}

template <typename T>
void ContiguousCircularList<T>::insert_node(const T& data, ContiguousCircularListNode<T>* prev) {
    ContiguousCircularListNode<T>* current = new ContiguousCircularListNode<T>(data, address_book);
    current->next = prev->next;
    current->prev = prev;
    prev->next->prev = current;
    prev->next = current;
}

template <typename T>
void ContiguousCircularList<T>::create_initial_node(const T& data) {
    head = new ContiguousCircularListNode<T>(data, address_book);
    head->prev = head;
    head->next = head;
}

template <typename T>
void ContiguousCircularList<T>::delete_node(const ContiguousCircularListNode<T>* node) {
    if (head == node) {
        head == node->prev;
    }
    address_book[node->index_in_address_book] = address_book[address_book.size() - 1];
    address_book.pop_back();
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
}

template <typename T>
ContiguousCircularListNode<T>* ContiguousCircularList<T>::random_node() const {
    // Assume address_book.size() is tiny compared to RAND_MAX
    size_t random_integer = std::rand() % address_book.size();
    return address_book[random_integer];
}

template <typename T>
size_t ContiguousCircularList<T>::size() const {
    return address_book.size();
}

#endif  // CONTIGUOUS_LIST_CPP
