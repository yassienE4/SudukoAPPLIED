#ifndef CUSTOM_PAIR_H
#define CUSTOM_PAIR_H

template <typename T1, typename T2>
class CustomPair {
public:
    T1 first;
    T2 second;
    
    // Constructors
    CustomPair() : first(T1()), second(T2()) {}
    CustomPair(const T1& f, const T2& s) : first(f), second(s) {}
    CustomPair(const CustomPair<T1, T2>& other) : first(other.first), second(other.second) {}
    
    // Assignment operator
    CustomPair<T1, T2>& operator=(const CustomPair<T1, T2>& other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }
    
    // Comparison operators
    bool operator==(const CustomPair<T1, T2>& other) const {
        return first == other.first && second == other.second;
    }
    
    bool operator!=(const CustomPair<T1, T2>& other) const {
        return !(*this == other);
    }
};

#endif // CUSTOM_PAIR_H
