#include "matrix.hpp"

size_t IterableSquareMatrix::size() const {
    return dim;
}

size_t IterableSquareMatrix::find_max(bool axis) const {
    size_t max_sum = 0;
    size_t vertex = 0;
    Iterator* begin, * end;
    for (size_t i = 0; i < dim; i++) {
        size_t sum_in_row = 0;
        if (axis == 1)
            begin = iterate_columns(i), end = end_columns(i);
        else begin = iterate_rows(i), end = end_rows(i);
        for (; begin != nullptr && *begin != *end; ++ * begin)
            sum_in_row += (**begin).second;
        if (sum_in_row > max_sum)
            max_sum = sum_in_row, vertex = i;
        delete begin;
        delete end;
    }
    return vertex;
}

size_t IterableSquareMatrix::max_inc_edges() const {
    return find_max(1);
}

size_t IterableSquareMatrix::max_outc_edges() const {
    return find_max(0);
}

void IterableSquareMatrix::deep1(size_t i, vector<bool>& visited,
            vector<size_t>& order) const {
    visited[i] = true;
    Iterator* begin = iterate_rows(i), * end = end_rows(i);
    size_t next_vertex;
    for (; begin != nullptr && *begin != *end; ++ * begin) {
        next_vertex = (**begin).first;
        if (!visited[next_vertex])
            deep1(next_vertex, visited, order);
    }
    order.push_back(i);
    delete begin;
    delete end;
}

void IterableSquareMatrix::deep2(size_t i, vector<bool>& visited) const {
    visited[i] = true;
    Iterator* begin = iterate_columns(i), * end = end_columns(i);
    size_t next_vertex;
    for (; begin != nullptr && *begin != *end; ++ * begin) {
        next_vertex = (**begin).first;
        if (!visited[next_vertex])
            deep2(next_vertex, visited);
    }
    delete begin;
    delete end;
}

size_t IterableSquareMatrix::strong_components_number() const {
    vector<bool> visited(dim);
    vector<size_t> order;
    size_t result = 0, v;
    for (size_t i = 0; i < dim; ++i)
        if (!visited[i])
            deep1(i, visited, order);
    visited.assign(dim, false);
    for (size_t i = 0; i < dim; ++i) {
        v = order[dim - 1 - i];
        if (!visited[v]) {
            deep2(v, visited);
            ++result;
        }
    }
    return result;
}