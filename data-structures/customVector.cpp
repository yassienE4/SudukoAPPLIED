#include "CustomVector.h"
#include "CustomPair.h"
#include <stdexcept>

// Implementation of all methods
template <typename T>
CustomVector<T>::CustomVector() : capacity(10), currentSize(0) {
    data = new T[capacity];
}

template <typename T>
CustomVector<T>::CustomVector(int initialCapacity) : capacity(initialCapacity), currentSize(0) {
    data = new T[capacity];
}

template <typename T>
CustomVector<T>::CustomVector(const CustomVector<T>& other) : capacity(other.capacity), currentSize(other.currentSize) {
    data = new T[capacity];
    for (int i = 0; i < currentSize; i++) {
        data[i] = other.data[i];
    }
}

template <typename T>
CustomVector<T>::~CustomVector() {
    delete[] data;
}

// Assignment operator
template <typename T>
CustomVector<T>& CustomVector<T>::operator=(const CustomVector<T>& other) {
    if (this != &other) {
        delete[] data;
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new T[capacity];
        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

// Resize the vector
template <typename T>
void CustomVector<T>::resize() {
    capacity *= 2;
    T* newData = new T[capacity];
    for (int i = 0; i < currentSize; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

// Element access with bounds checking
template <typename T>
T& CustomVector<T>::at(int index) {
    if (index < 0 || index >= currentSize) {
        throw "Index out of range";
    }
    return data[index];
}

// Const element access with bounds checking
template <typename T>
const T& CustomVector<T>::at(int index) const {
    if (index < 0 || index >= currentSize) {
        throw "Index out of range";
    }
    return data[index];
}

// Element access without bounds checking
template <typename T>
T& CustomVector<T>::operator[](int index) {
    return data[index];
}

// Const element access without bounds checking
template <typename T>
const T& CustomVector<T>::operator[](int index) const {
    return data[index];
}

// Get the size of the vector
template <typename T>
int CustomVector<T>::size() const {
    return currentSize;
}

// Check if the vector is empty
template <typename T>
bool CustomVector<T>::empty() const {
    return currentSize == 0;
}

// Add an element to the end
template <typename T>
void CustomVector<T>::push_back(const T& value) {
    if (currentSize >= capacity) {
        resize();
    }
    data[currentSize++] = value;
}

// Remove the last element
template <typename T>
void CustomVector<T>::pop_back() {
    if (currentSize > 0) {
        currentSize--;
    }
}

// Clear all elements
template <typename T>
void CustomVector<T>::clear() {
    currentSize = 0;
}

// Explicit instantiation for the types you need
template class CustomVector<int>;
template class CustomVector<CustomPair<int, int>>;