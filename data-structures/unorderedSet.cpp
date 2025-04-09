#include "unorderedSet.h"
#include <iostream>

// Hash function implementation
int unorderedSet::hashFunction(int key) const {
    return key % TABLE_SIZE;
}

// Iterator implementation
void unorderedSet::Iterator::findNext() {
    while (currentIndex < TABLE_SIZE) {
        if (currentNode != nullptr) {
            return; // Found a valid node
        }
        
        // Move to the next bucket
        currentIndex++;
        if (currentIndex < TABLE_SIZE) {
            currentNode = set->table[currentIndex];
        }
    }
}

unorderedSet::Iterator::Iterator(const unorderedSet* s, bool begin) : set(s), currentIndex(0), currentNode(nullptr) {
    if (begin) {
        // Find the first valid node
        currentNode = set->table[0];
        if (currentNode == nullptr) {
            findNext();
        }
    } else {
        // End iterator points past the last bucket
        currentIndex = TABLE_SIZE;
    }
}

int unorderedSet::Iterator::operator*() const {
    if (currentNode == nullptr) {
        throw std::runtime_error("Dereferencing invalid iterator");
    }
    return currentNode->key;
}

unorderedSet::Iterator& unorderedSet::Iterator::operator++() {
    if (currentNode != nullptr) {
        currentNode = currentNode->next;
        if (currentNode == nullptr) {
            findNext();
        }
    }
    return *this;
}

bool unorderedSet::Iterator::operator!=(const Iterator& other) const {
    return currentIndex != other.currentIndex || currentNode != other.currentNode;
}

// Constructor
unorderedSet::unorderedSet() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

// Destructor
unorderedSet::~unorderedSet() {
    clear();
}

// Copy constructor
unorderedSet::unorderedSet(const unorderedSet& other) {
    // Initialize all buckets to nullptr
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
    
    // Copy all elements from other
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = other.table[i];
        while (current) {
            insert(current->key);
            current = current->next;
        }
    }
}

// Assignment operator
unorderedSet& unorderedSet::operator=(const unorderedSet& other) {
    if (this != &other) {
        // Clear current contents
        clear();
        
        // Copy all elements from other
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = other.table[i];
            while (current) {
                insert(current->key);
                current = current->next;
            }
        }
    }
    return *this;
}

// Insert a key into the set
void unorderedSet::insert(int key) {
    int index = hashFunction(key);
    
    // Check if key already exists
    Node* current = table[index];
    while (current) {
        if (current->key == key) {
            return; // Key already exists
        }
        current = current->next;
    }
    
    // Insert new key
    Node* newNode = new Node(key);
    newNode->next = table[index];
    table[index] = newNode;
}

// Check if a key exists in the set
bool unorderedSet::contains(int key) const {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Remove a key from the set
void unorderedSet::remove(int key) {
    int index = hashFunction(key);
    Node* current = table[index];
    Node* prev = nullptr;
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Clear all elements
void unorderedSet::clear() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        table[i] = nullptr;
    }
}

// Get the number of elements in the set
int unorderedSet::size() const {
    int count = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current) {
            count++;
            current = current->next;
        }
    }
    return count;
}

// Check if the set is empty
bool unorderedSet::empty() const {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i] != nullptr) {
            return false;
        }
    }
    return true;
}

// Get iterator to the beginning
unorderedSet::Iterator unorderedSet::begin() const {
    return Iterator(this, true);
}

// Get iterator to the end
unorderedSet::Iterator unorderedSet::end() const {
    return Iterator(this, false);
}
