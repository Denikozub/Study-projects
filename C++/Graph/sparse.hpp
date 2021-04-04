#ifndef SPARSE_HPP
#define SPARSE_HPP

#include <map>
#include "matrix.hpp"

using std::map;

//A LOOP INCREASES MATRIX[I][I] BY 1

class ConstSparseRowIterator : public Iterator {
    map<size_t, size_t>::const_iterator it;
public:
    ConstSparseRowIterator(const map<size_t, size_t>::const_iterator&);
    pair<size_t, size_t> operator* () const override;
    ConstSparseRowIterator& operator++ () override;
    bool operator== (const Iterator&) override;
    bool operator!= (const Iterator&) override;
};

class ConstSparseColumnIterator : public Iterator {
    size_t col;
    map<size_t, map<size_t, size_t>>::const_iterator row, row_end;
public:
    ConstSparseColumnIterator(size_t, map<size_t, map<size_t, size_t>>::
            const_iterator, map<size_t, map<size_t, size_t>>::const_iterator);
    pair<size_t, size_t> operator* () const override;
    ConstSparseColumnIterator& operator++ () override;
    bool operator== (const Iterator&) override;
    bool operator!= (const Iterator&) override;
};

class IterableSparseMatrix : public IterableSquareMatrix {
    map<size_t, map<size_t, size_t>> matrix;
public:
    IterableSparseMatrix(const map<size_t, map<size_t, size_t>>&, size_t);
    ConstSparseRowIterator* iterate_rows(size_t) const override;
    ConstSparseColumnIterator* iterate_columns(size_t) const override;
    ConstSparseRowIterator* end_rows(size_t) const override;
    ConstSparseColumnIterator* end_columns(size_t) const override;
};

#endif
