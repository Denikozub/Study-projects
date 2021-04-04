#ifndef SPARSE_VECTOR_HPP
#define SPARSE_VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <cmath>
#include "list.hpp"

using std::out_of_range;
using std::invalid_argument;


/*
template class of SparseVector
uses template class List to store a list of non-default elements
precision of floating point comparison set by epsilon parameter
default value can be set by default_value parameter
indexing operators [] and * for non-constant objects allocate memory for
... a default object (ex cout << v[0])
NO cycles through default elements
*/

template <class T>
class SparseVector {
    List<T> vector;
    size_t number_of_columns;
    T default_value;
    double epsilon;
    size_t request = 0; //default operator* indexing

public:
    SparseVector() {}
    SparseVector(size_t columns, T set_default_value,
            double set_epsilon = 0.0000001);
    SparseVector(size_t col_number, T value, size_t columns,
            T set_default_value, double set_epsilon = 0.0000001);

    void set(size_t col_number, T value);

    // check if vector with index exists
    bool exists(size_t col_number) const;

    // get data of a vector after a vector with index
    // if index == -1 return data of first vector
    // if vector not found or is last return -2
    T& get_next(long int& index) const;
    T get(size_t col_number) const;

    T& operator[] (size_t col_number);
    T operator[] (size_t col_number) const;
    SparseVector<T>& operator+ (size_t col_number);
    template <typename U>
    friend SparseVector<U>& operator+ (size_t, SparseVector<U>&);
    SparseVector<T> operator+ (size_t col_number) const;
    template <typename U>
    friend SparseVector<U> operator+ (size_t, const SparseVector<U>&);
    T& operator* ();
    T operator* () const;
    bool operator== (const SparseVector<T>& obj) const;
    bool operator!= (const SparseVector<T>& obj) const;
};


template <typename T>
SparseVector<T>::SparseVector(size_t columns,
        T set_default_value, double set_epsilon) {
    if (columns <= 0)
        throw invalid_argument("incorrect vector size");
    if (set_epsilon <= 0)
        throw invalid_argument("incorrect epsilon");
    number_of_columns = columns;
    default_value = set_default_value;
    epsilon = set_epsilon;
}

template <typename T>
SparseVector<T>::SparseVector(size_t col_number, T value,
        size_t columns, T set_default_value, double set_epsilon) {
    if (col_number + 1 > columns)
        throw out_of_range("vector index out of range");
    if (columns <= 0)
        throw invalid_argument("incorrect vector size");
    if (set_epsilon <= 0)
        throw invalid_argument("incorrect epsilon");
    vector.add(col_number, value);
    number_of_columns = columns;
    default_value = set_default_value;
    epsilon = set_epsilon;
}

template <typename T>
void SparseVector<T>::set(size_t col_number, T value) {
    if (col_number + 1 > number_of_columns)
        throw out_of_range("set(): matrix index out of range");
    if (vector.exists(col_number))
        vector[col_number] = value;
    else vector.add(col_number, value);
}

template <typename T>
T SparseVector<T>::get(size_t col_number) const {
    if (col_number + 1 > number_of_columns)
        throw out_of_range("get(): matrix index out of range");
    return vector.exists(col_number) ? vector[col_number] : default_value;
}

template <typename T>
bool SparseVector<T>::exists(size_t col_number) const {
    return vector.exists(col_number);
}

template <typename T>
T& SparseVector<T>::get_next(long int& index) const {
    return vector.get_next(index);
}

template <typename T>
T& SparseVector<T>::operator[] (size_t col_number) {
    if (col_number + 1 > number_of_columns)
        throw out_of_range("operator[] matrix index out of range");
    return vector.exists(col_number) ? vector[col_number] : 
            vector.add(col_number, default_value);
}

template <typename T>
T SparseVector<T>::operator[] (size_t col_number) const {
    return get(col_number);
}

template <typename T>
SparseVector<T>& SparseVector<T>::operator+ (size_t col_number) {
    request += col_number;
    return *this;
}

template <typename U>
SparseVector<U>& operator+ (size_t line_number, SparseVector<U>& obj) {
    obj.request += line_number;
    return obj;
}

template <typename T>
SparseVector<T> SparseVector<T>::operator+ (size_t col_number) const {
    SparseVector<T> temp(*this);
    temp.request += col_number;
    return temp;
}

template <typename U>
SparseVector<U> operator+ (size_t line_number, const SparseVector<U>& obj) {
    SparseVector<U> temp(obj);
    temp.request += line_number;
    return temp;
}

template <typename T>
T& SparseVector<T>::operator* () {
    size_t temp = request; request = 0;
    return operator[](temp);
}

template <typename T>
T SparseVector<T>::operator* () const {
    return get(request);
}

template <typename T>
bool SparseVector<T>::operator== (const SparseVector<T>& obj) const {
    return (size_t)number_of_columns != (size_t)obj.number_of_columns ||
            default_value != obj.default_value || request != obj.request ?
            false : vector.compare(obj.vector, epsilon);
}

template <>
bool SparseVector<double>::operator== (const SparseVector<double>& obj) const {
    return (size_t)number_of_columns != (size_t)obj.number_of_columns ||
            std::fabs(default_value - obj.default_value) > epsilon ||
            request != obj.request ? false : vector.compare(obj.vector, epsilon);
}

template <typename T>
bool SparseVector<T>::operator!= (const SparseVector<T>& obj) const {
    return !operator==(obj);
}

#endif
