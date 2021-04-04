#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <vector>
#include "iterator.hpp"
using std::vector;

class IterableSquareMatrix {
    size_t find_max(bool) const;
    void deep1(size_t, vector<bool>&, vector<size_t>&) const;
    void deep2(size_t, vector<bool>&) const;
protected:
    size_t dim;
public:
    virtual Iterator* iterate_rows(size_t) const = 0;
    virtual Iterator* iterate_columns(size_t) const = 0;
    virtual Iterator* end_rows(size_t) const = 0;
    virtual Iterator* end_columns(size_t) const = 0;
    size_t size() const;
    size_t max_inc_edges() const;
    size_t max_outc_edges() const;
    size_t strong_components_number() const;
    ~IterableSquareMatrix() = default;
};

#endif
