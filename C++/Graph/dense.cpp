#include <stdexcept>
#include "dense.hpp"

using std::out_of_range;
using std::invalid_argument;

/****************************ConstDenseRowIterator****************************/

ConstDenseRowIterator::ConstDenseRowIterator (const vector<vector<size_t>>*
        mat, size_t set_row, size_t set_dim, size_t set_col) :
        matrix_p(mat), row(set_row), dim(set_dim), col(set_col) {
    while (col != dim && matrix_p->at(row).at(col) == 0) ++col;
}

pair<size_t, size_t> ConstDenseRowIterator::operator* () const {
    return pair<size_t, size_t> {col, matrix_p->at(row).at(col)};
}

ConstDenseRowIterator& ConstDenseRowIterator::operator++ () {
    ++col;
    while (col != dim && matrix_p->at(row).at(col) == 0) ++col; 
    return *this;
}

bool ConstDenseRowIterator::operator== (const Iterator& obj) {
    auto a = dynamic_cast<const ConstDenseRowIterator&>(obj);
    return (a.matrix_p == matrix_p) && (a.col == col) && (a.row == row);
}

bool ConstDenseRowIterator::operator!= (const Iterator& obj) {
    return !operator==(obj);
}


/**************************ConstDenseColumnIterator***************************/

ConstDenseColumnIterator::ConstDenseColumnIterator(const vector<vector<size_t>>*
        mat, size_t set_col, size_t set_dim, size_t set_row) :
        matrix_p(mat), col(set_col), dim(set_dim), row(set_row) {
    while (row != dim && matrix_p->at(row).at(col) == 0) ++row;
}

pair<size_t, size_t> ConstDenseColumnIterator::operator* () const {
    return pair<size_t, size_t> {row, matrix_p->at(row).at(col)};
}

ConstDenseColumnIterator& ConstDenseColumnIterator::operator++ () {
    ++row;
    while (row != dim && matrix_p->at(row).at(col) == 0) ++row;
    return *this;
}

bool ConstDenseColumnIterator::operator== (const Iterator& obj) {
    auto a = dynamic_cast<const ConstDenseColumnIterator&>(obj);
    return (a.matrix_p == matrix_p) && (a.col == col) && (a.row == row);
}

bool ConstDenseColumnIterator::operator!= (const Iterator& obj) {
    return !operator==(obj);
}


/*****************************IterableDenseMatrix*****************************/

IterableDenseMatrix::IterableDenseMatrix
        (const vector<vector<size_t>>& src, size_t set_dim) : matrix(src) {
    dim = set_dim;
    if (dim == 0) throw invalid_argument("matrix is empty");
    if (dim != src.size()) throw invalid_argument("dim is incorrect");
    for (auto& i : matrix)
        if (i.size() != dim) throw invalid_argument("matrix is not square");
}

ConstDenseRowIterator* IterableDenseMatrix::iterate_rows(size_t pos) const {
    if (pos >= dim) throw out_of_range("no such row");
    return new ConstDenseRowIterator(&matrix, pos, dim);
}

ConstDenseColumnIterator* IterableDenseMatrix::iterate_columns(size_t pos)const{
    if (pos >= dim) throw out_of_range("no such column");
    return new ConstDenseColumnIterator(&matrix, pos, dim);
}

ConstDenseRowIterator* IterableDenseMatrix::end_rows(size_t pos) const {
    if (pos >= dim) throw out_of_range("no such row");
    return new ConstDenseRowIterator(&matrix, pos, dim, dim);
}

ConstDenseColumnIterator* IterableDenseMatrix::end_columns(size_t pos) const {
    if (pos >= dim) throw out_of_range("no such column");
    return new ConstDenseColumnIterator(&matrix, pos, dim, dim);
}
