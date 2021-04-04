#ifndef DENSE_HPP
#define DENSE_HPP

#include "matrix.hpp"


//A LOOP INCREASES MATRIX[I][I] BY 1

class ConstDenseRowIterator : public Iterator {
    const vector<vector<size_t>>* matrix_p;
    size_t row, dim, col;
public:
    ConstDenseRowIterator(const vector<vector<size_t>>*,
            size_t, size_t, size_t = 0);
    pair<size_t, size_t> operator* () const override;
    ConstDenseRowIterator& operator++ () override;
    bool operator== (const Iterator&) override;
    bool operator!= (const Iterator&) override;
};

class ConstDenseColumnIterator : public Iterator {
    const vector<vector<size_t>>* matrix_p;
    size_t col, dim, row;
public:
    ConstDenseColumnIterator(const vector<vector<size_t>>*,
            size_t, size_t, size_t = 0);
    pair<size_t, size_t> operator* () const override;
    ConstDenseColumnIterator& operator++ () override;
    bool operator== (const Iterator&) override;
    bool operator!= (const Iterator&) override;
};

class IterableDenseMatrix : public IterableSquareMatrix {
    vector<vector<size_t>> matrix;
public:
    IterableDenseMatrix(const vector<vector<size_t>>&, size_t);
    ConstDenseRowIterator* iterate_rows(size_t) const override;
    ConstDenseColumnIterator* iterate_columns(size_t) const override;
    ConstDenseRowIterator* end_rows(size_t) const override;
    ConstDenseColumnIterator* end_columns(size_t) const override;
};

#endif
