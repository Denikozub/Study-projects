#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <cstddef>
#include <stdexcept>
#include <cmath>
#include "sparse_vector.hpp"
#include "list.hpp"

using std::out_of_range;
using std::invalid_argument;


/*
template class of SparseMatrix
uses template class List to store a list of non-default lines
uses template class SparseVector to store a list of non-default elements
precision of floating point comparison set by epsilon parameter
default value can be set by default_value parameter
indexing operators [] and * for non-constant objects allocate memory for
... a default object (ex cout << m[0][0])
sum and multiply operators + and * do NOT allocate memory
NO cycles through default elements
*/

template <class T>
class SparseMatrix {
    List<SparseVector<T>> matrix;
    size_t number_of_lines;
    size_t number_of_columns;
    T default_value;
    double epsilon;
    size_t request = 0; //default operator* indexing
    void set_sum(const SparseMatrix<T>& obj1, const SparseMatrix<T>& obj2);
    void set_mult(const SparseMatrix<T>& obj1,
            const SparseMatrix<T>& obj2, T& two_elements_mult);

public:
    explicit SparseMatrix(size_t lines, size_t columns,
            T set_default_value, double set_epsilon = 0.0000001);

    void set(size_t line_number, size_t col_number, T value);
    size_t num_rows() const;
    size_t num_columns() const;
    T get(size_t line_number, size_t col_number) const;

    SparseVector<T>& operator[] (size_t line_number);
    SparseVector<T> operator[] (size_t line_number) const;
    SparseMatrix<T>& operator+ (size_t line_number);
    template <typename U>
    friend SparseMatrix<U>& operator+ (size_t, SparseMatrix<U>&);
    SparseMatrix<T> operator+ (size_t line_number) const;
    template <typename U>
    friend SparseMatrix<U> operator+ (size_t, const SparseMatrix<U>&);
    SparseVector<T>& operator* ();
    SparseVector<T> operator* () const;
    bool operator== (const SparseMatrix<T>& obj) const;
    bool operator!= (const SparseMatrix<T>& obj) const;
    template <typename U> friend const SparseMatrix<U> operator+
            (const SparseMatrix<U>& obj1, const SparseMatrix<U>& obj2);
    template <typename U> friend const SparseMatrix<U> operator*
            (const SparseMatrix<U>& obj1, const SparseMatrix<U>& obj2);

};


template <typename T>
SparseMatrix<T>::SparseMatrix(size_t lines, size_t columns,
        T set_default_value, double set_epsilon) {
    if (lines <= 0 || columns <= 0)
        throw invalid_argument("incorrect matrix size");
    if (set_epsilon <= 0)
        throw invalid_argument("incorrect epsilon");
    number_of_lines = lines;
    number_of_columns = columns;
    default_value = set_default_value;
    epsilon = set_epsilon;
}

template <typename T>
size_t SparseMatrix<T>::num_rows() const {
    return number_of_lines;
}

template <typename T>
size_t SparseMatrix<T>::num_columns() const {
    return number_of_columns;
}

template <typename T>
void SparseMatrix<T>::set(size_t line_number, size_t col_number, T value) {
    if (line_number + 1 > number_of_lines || col_number + 1 > number_of_columns)
        throw out_of_range("set(): matrix index out of range");
    if (matrix.exists(line_number))
        matrix[line_number].set(col_number, value);
    else matrix.add(line_number, SparseVector<T>(col_number, value,
            number_of_columns, default_value, epsilon));
}

template <typename T>
T SparseMatrix<T>::get(size_t line_number, size_t col_number) const {
    if (line_number + 1 > number_of_lines || col_number + 1 > number_of_columns)
        throw out_of_range("get(): matrix index out of range");
    return matrix.exists(line_number) ?
            matrix[line_number].get(col_number) : default_value;
}

template <typename T>
SparseVector<T>& SparseMatrix<T>::operator[] (size_t line_number) {
    if (line_number + 1 > number_of_lines)
        throw out_of_range("operator[] matrix index out of range");
    return matrix.exists(line_number) ?
            matrix[line_number] : matrix.add(line_number,
            SparseVector<T>(number_of_columns, default_value, epsilon));
}

template <typename T>
SparseVector<T> SparseMatrix<T>::operator[] (size_t line_number) const {
    if (line_number + 1 > number_of_lines)
        throw out_of_range("operator[] matrix index out of range");
    return matrix.exists(line_number) ? matrix[line_number] :
            SparseVector<T>(number_of_columns, default_value, epsilon);
}

template <typename T>
SparseMatrix<T>& SparseMatrix<T>::operator+ (size_t line_number) {
    request += line_number;
    return *this;
}

template <typename U>
SparseMatrix<U>& operator+ (size_t line_number, SparseMatrix<U>& obj) {
    obj.request += line_number;
    return obj;
}

template <typename T>
SparseMatrix<T> SparseMatrix<T>::operator+ (size_t line_number) const {
    SparseMatrix<T> temp(*this);
    temp.request += line_number;
    return temp;
}

template <typename U>
SparseMatrix<U> operator+ (size_t line_number, const SparseMatrix<U>& obj) {
    SparseMatrix<U> temp(obj);
    temp.request += line_number;
    return temp;
}

template <typename T>
SparseVector<T>& SparseMatrix<T>::operator* () {
    size_t temp = request; request = 0;
    return operator[](temp);
}

template <typename T>
SparseVector<T> SparseMatrix<T>::operator* () const {
    return operator[](request);
}

template <typename T>
bool SparseMatrix<T>::operator== (const SparseMatrix<T>& obj) const {
    return (size_t)number_of_lines != (size_t)obj.number_of_lines ||
            (size_t)number_of_columns != (size_t)obj.number_of_columns ||
            default_value != obj.default_value ||
            request != obj.request ? false : matrix.compare(obj.matrix);
}

template <>
bool SparseMatrix<double>::operator== (const SparseMatrix<double>& obj) const {
    return (size_t)number_of_lines != (size_t)obj.number_of_lines ||
            (size_t)number_of_columns != (size_t)obj.number_of_columns ||
            std::fabs(default_value - obj.default_value) > epsilon ||
            request != obj.request ? false : matrix.compare(obj.matrix);
}

template <typename T>
bool SparseMatrix<T>::operator!= (const SparseMatrix<T>& obj) const {
    return !operator==(obj);
}

template <typename U>
const SparseMatrix<U> operator+
        (const SparseMatrix<U>& obj1, const SparseMatrix<U>& obj2) {
    if (obj1.number_of_lines != obj2.number_of_lines ||
        obj1.number_of_columns != obj2.number_of_columns)
        throw invalid_argument("operator+ incompatible sizes");
    SparseMatrix<U> temp(obj1.number_of_lines, obj1.number_of_columns,
            obj1.default_value + obj2.default_value, obj1.epsilon);
    temp.set_sum(obj1, obj2);
    return temp;
}

template <typename T>
void SparseMatrix<T>::set_sum(const SparseMatrix<T>& obj1,
                             const SparseMatrix<T>& obj2) {
    SparseVector<T> found_vector;
    if (obj1.matrix.length() == 0 || obj2.matrix.length() == 0)
        return; // one of matrixes is empty
    long int i, j;
    T found_element;
    i = -1; while (1) { // getting lines of matrix1
        found_vector = obj1.matrix.get_next(i);
        if (i == -2) break; // lines ended
        j = -1; while (1) { // getting elements from line of matrix1
            found_element = found_vector.get_next(j);
            if (j == -2) break; // elements ended
            (*this)[i][j] = found_element + obj2.get(i, j);
        }
    }
    i = -1; while (1) { // getting lines of matrix2
        found_vector = obj2.matrix.get_next(i);
        if (i == -2) break; // lines ended
        j = -1; while (1) { // getting elements from line of matrix2
            found_element = found_vector.get_next(j);
            if (j == -2) break; // elements ended
            (*this)[i][j] = found_element + obj1.get(i, j);
        }
    }
}

template <typename U>
const SparseMatrix<U> operator*
        (const SparseMatrix<U>& obj1, const SparseMatrix<U>& obj2) {
    if (obj1.number_of_columns != obj2.number_of_lines)
        throw invalid_argument("operator* incompatible sizes");
    U two_elements_mult = obj1.default_value * obj2.default_value;
    SparseMatrix<U> temp(obj1.number_of_lines, obj2.number_of_columns,
            obj1.number_of_columns * two_elements_mult, obj1.epsilon);
    temp.set_mult(obj1, obj2, two_elements_mult);
    return temp;
}

template <typename T>
void SparseMatrix<T>::set_mult(const SparseMatrix<T>& obj1,
        const SparseMatrix<T>& obj2, T& two_elements_mult) {
    SparseVector<T> found_vector1, found_vector2;
    if (obj1.matrix.length() == 0 || obj2.matrix.length() == 0)
        return; // one of matrixes is empty
    long int i = -1, k, j;
    T found_element;
    while (1) { // getting lines of matrix1
        found_vector1 = obj1.matrix.get_next(i);
        if (i == -2) break; // lines ended
        k = -1;
        while (1) { // getting lines of matrix2
            found_vector2 = obj2.matrix.get_next(k);
            if (k == -2) break; // lines ended
            // check if line of matrix2 has elements in
            // column of the same line that was found in matrix2
            if (!found_vector1.exists(k)) continue; 
            j = -1;
            while (1) { // getting elements from line of matrix2
                found_element = found_vector2.get_next(j);
                if (j == -2) break; // elements ended
                T& temp_data = (*this)[i][j];
                temp_data = temp_data + found_vector1[k] *
                        found_element - two_elements_mult;
            }
        }
    }
}

#endif
