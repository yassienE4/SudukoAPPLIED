#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "customPair.h"

class hashtable {
private:
    struct node {
        CustomPair<int, int> data;
        node* next;
        node(int key, int value) : data(key, value), next(nullptr) {}
    };

    static const int table_size = 101;
    node* table[table_size];

    int hash(int key) const;

public:
    hashtable();
    ~hashtable();

    void insert(int key, int value);
    int get(int key) const;
    bool contains(int key) const;
    void remove(int key);
    void clear();
};

#endif // HASHTABLE_H


