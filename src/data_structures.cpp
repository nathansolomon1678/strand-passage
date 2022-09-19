#include "data_structures.h"

// Typically .cpp files don't need header guards, but since there's weird
// template stuff, the corresponding header file is including this file
#ifndef DATA_STRUCTURES_CPP
#define DATA_STRUCTURES_CPP

template <typename T>
ForwardListNode<T>::ForwardListNode(const T& value, ForwardListNode<T>* next) :
    value(value), next(next) {
}

/* See header file for documentation on ContiguousCircularListNode and ContiguousCircularList */

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
        head = node->next;
    }
    address_book[address_book.size() - 1]->index_in_address_book = node->index_in_address_book;
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

////////////////////////////////////////////////////////////////////////////////
// See documentation for the iVec3_hashtable class in the header file
////////////////////////////////////////////////////////////////////////////////

inline iVec3_hashtable::~iVec3_hashtable() {
    // To avoid memory leaks, the destructor must also remove the chains
    // (alternatively, this could just use smart pointers)
    for (int i = 0; i < IVEC3_HASHTABLE_WIDTH; ++i) {
    for (int j = 0; j < IVEC3_HASHTABLE_WIDTH; ++j) {
    for (int k = 0; k < IVEC3_HASHTABLE_WIDTH; ++k) {
        // Keeps track of two pointers in order to delete elements of the list
        // in forward order
        ForwardListNode<ivec3>* current = data[i][j][k];
        ForwardListNode<ivec3>* prev;
        while (current != nullptr) {
            prev = current;
            current = current->next;
            delete prev;
        }
    }
    }
    }
}

inline bool iVec3_hashtable::contains(const ivec3& key) {
    // Use an process resembling what a hash function would use to take the
    // value of coord and get valid indices from that. Note that this assumes
    // width is a power of 2 (since it uses bitwise AND instead of modulo)
    ForwardListNode<ivec3>* chain_ptr = data[key[0] & (IVEC3_HASHTABLE_WIDTH - 1)]
                                            [key[1] & (IVEC3_HASHTABLE_WIDTH - 1)]
                                            [key[2] & (IVEC3_HASHTABLE_WIDTH - 1)];
    while (chain_ptr != nullptr) {
        if (chain_ptr->value == key) {
            return true;
        }
        chain_ptr = chain_ptr->next;
    }
    return false;
}

inline void iVec3_hashtable::insert(const ivec3& key) {
    ForwardListNode<ivec3>* chain_ptr = data[key[0] & (IVEC3_HASHTABLE_WIDTH - 1)]
                                            [key[1] & (IVEC3_HASHTABLE_WIDTH - 1)]
                                            [key[2] & (IVEC3_HASHTABLE_WIDTH - 1)];
    while (chain_ptr != nullptr) {
        chain_ptr = chain_ptr->next;
    }
    chain_ptr = new ForwardListNode<ivec3>(key, nullptr);
}

inline void iVec3_hashtable::erase(const ivec3& key) {
    ForwardListNode<ivec3>* chain_ptr = data[key[0] & (IVEC3_HASHTABLE_WIDTH - 1)]
                                            [key[1] & (IVEC3_HASHTABLE_WIDTH - 1)]
                                            [key[2] & (IVEC3_HASHTABLE_WIDTH - 1)];
    while (chain_ptr->value != key) {
        chain_ptr = chain_ptr->next;
    }
    ForwardListNode<ivec3>* next = chain_ptr->next;
    delete chain_ptr;
    chain_ptr = next;
}

#endif  // DATA_STRUCTURES_CPP
