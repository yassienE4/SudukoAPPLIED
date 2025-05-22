#ifndef CUSTOM_VECTOR_H
#define CUSTOM_VECTOR_H

template <typename T>
class CustomVector {
private:
    T* data;
    int capacity;
    int currentSize;

    void resize();

public:
    CustomVector();
    CustomVector(int initialCapacity);
    CustomVector(const CustomVector<T>& other);
    ~CustomVector();
    
    CustomVector<T>& operator=(const CustomVector<T>& other);
    
    T& operator[](int index);
    const T& operator[](int index) const;
    T& at(int index);
    const T& at(int index) const;
    
    int size() const;
    bool empty() const;
    
    void push_back(const T& value);
    void pop_back();
    void clear();
};

#endif // CUSTOM_VECTOR_H
