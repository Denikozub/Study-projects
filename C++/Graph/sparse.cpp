#include <stdexcept>
#include "sparse.hpp"

using std::out_of_range;
using std::invalid_argument;

/***************************ConstSparseRowIterator****************************/

ConstSparseRowIterator::ConstSparseRowIterator
        (const map<size_t, size_t>::const_iterator& src) : it(src) {}

pair<size_t, size_t> ConstSparseRowIterator::operator* () const {
    return pair<size_t, size_t> {it->first, it->second};
}

ConstSparseRowIterator& ConstSparseRowIterator::operator++ () {
    ++it; return *this;
}

bool ConstSparseRowIterator::operator== (const Iterator& obj) {
    auto a = dynamic_cast<const ConstSparseRowIterator&>(obj);
    return a.it == it;
}

bool ConstSparseRowIterator::operator!= (const Iterator& obj) {
    return !operator==(obj);
}


/*************************ConstSparseColumnIterator***************************/

ConstSparseColumnIterator::ConstSparseColumnIterator(size_t set_col,
        map<size_t, map<size_t, size_t>>::const_iterator set_row,
        map<size_t, map<size_t, size_t>>::const_iterator set_row_end) : 
        col(set_col), row(set_row), row_end(set_row_end) {
    while (row != row_end && row->second.find(col) == row->second.end()) ++row;
}

pair<size_t, size_t> ConstSparseColumnIterator::operator* () const {
    return pair<size_t, size_t> {row->first, row->second.find(col)->second};
}

ConstSparseColumnIterator& ConstSparseColumnIterator::operator++ () {
    ++row;
    while (row != row_end && row->second.find(col) == row->second.end()) ++row;
    return *this;
}

bool ConstSparseColumnIterator::operator== (const Iterator& obj) {
    auto a = dynamic_cast<const ConstSparseColumnIterator&>(obj);
    return (a.col == col) && (a.row == row);
}

bool ConstSparseColumnIterator::operator!= (const Iterator& obj) {
    return !operator==(obj);
}


/****************************IterableSparseMatrix*****************************/

IterableSparseMatrix::IterableSparseMatrix (const map<size_t, map<size_t,
        size_t>>& src, size_t set_dim) : matrix(src) {
    dim = set_dim;
    if (dim == 0) throw invalid_argument("matrix is empty");
    if (matrix.upper_bound(dim) != matrix.end())
        throw invalid_argument("dim is incorrect");
    for (auto& i : matrix)
        if (i.second.upper_bound(dim) != i.second.end())
            throw invalid_argument("dim is incorrect");
}

ConstSparseRowIterator* IterableSparseMatrix::iterate_rows(size_t pos) const {
    if (pos >= dim) throw out_of_range("no such row");
    if (matrix.find(pos) == matrix.end()) return nullptr;
    return new ConstSparseRowIterator(matrix.find(pos)->second.cbegin());
}

ConstSparseColumnIterator*
        IterableSparseMatrix::iterate_columns(size_t pos) const {
    if (pos >= dim) throw out_of_range("no such column");
    return new ConstSparseColumnIterator(pos, matrix.begin(), matrix.end());
}

ConstSparseRowIterator* IterableSparseMatrix::end_rows(size_t pos) const {
    if (pos >= dim) throw out_of_range("no such row");
    if (matrix.find(pos) == matrix.end()) return nullptr;
    return new ConstSparseRowIterator(matrix.find(pos)->second.cend());
}

ConstSparseColumnIterator* IterableSparseMatrix::end_columns(size_t pos) const {
    if (pos >= dim) throw out_of_range("no such column");
    return new ConstSparseColumnIterator(pos, matrix.end(), matrix.end());
}
