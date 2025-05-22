#ifndef UNORDEREDSET_H
#define UNORDEREDSET_H

class unorderedSet {
private:
    static const int TABLE_SIZE = 1000; // Size of the hash table
    struct Node {
        int key;
        Node* next;
        Node(int k) : key(k), next(nullptr) {}
    };
    Node* table[TABLE_SIZE];

    // Hash function
    int hashFunction(int key) const;

public:
    // Iterator for traversing the set
    class Iterator {
    private:
        const unorderedSet* set;
        int currentIndex;
        Node* currentNode;
        
        // Find the next valid node
        void findNext();
        
    public:
        Iterator(const unorderedSet* s, bool begin);
        int operator*() const;
        Iterator& operator++();
        bool operator!=(const Iterator& other) const;
    };

    unorderedSet();
    ~unorderedSet();
    
    // Copy constructor
    unorderedSet(const unorderedSet& other);
    
    // Assignment operator
    unorderedSet& operator=(const unorderedSet& other);
    
    // Basic operations
    void insert(int key);
    bool contains(int key) const;
    void remove(int key);
    void clear();
    int size() const;
    bool empty() const;
    
    // Iterator support
    Iterator begin() const;
    Iterator end() const;
};

#endif // UNORDERED_SET_H
