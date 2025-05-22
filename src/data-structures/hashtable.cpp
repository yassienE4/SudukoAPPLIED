#include "hashtable.h"

hashtable::hashtable() {
    for (int i = 0; i < table_size; i++)
        table[i] = nullptr;
}

hashtable::~hashtable() {
    clear();
}

int hashtable::hash(int key) const {
    return key % table_size;
}

void hashtable::insert(int key, int value) {
    int idx = hash(key);
    node* curr = table[idx];

    while (curr) {
        if (curr->data.first == key) {
            curr->data.second = value;
            return;
        }
        curr = curr->next;
    }

    node* new_node = new node(key, value);
    new_node->next = table[idx];
    table[idx] = new_node;
}

int hashtable::get(int key) const {
    int idx = hash(key);
    node* curr = table[idx];

    while (curr) {
        if (curr->data.first == key)
            return curr->data.second;
        curr = curr->next;
    }

    return -1; // not found
}

bool hashtable::contains(int key) const {
    int idx = hash(key);
    node* curr = table[idx];

    while (curr) {
        if (curr->data.first == key)
            return true;
        curr = curr->next;
    }

    return false;
}

void hashtable::remove(int key) {
    int idx = hash(key);
    node* curr = table[idx];
    node* prev = nullptr;

    while (curr) {
        if (curr->data.first == key) {
            if (prev)
                prev->next = curr->next;
            else
                table[idx] = curr->next;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void hashtable::clear() {
    for (int i = 0; i < table_size; i++) {
        node* curr = table[i];
        while (curr) {
            node* temp = curr;
            curr = curr->next;
            delete temp;
        }
        table[i] = nullptr;
    }
}
