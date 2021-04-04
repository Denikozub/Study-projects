#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <utility>
using std::pair;

class Iterator {
public:
    virtual pair<size_t, size_t> operator* () const = 0;
    virtual Iterator& operator++ () = 0;
    virtual bool operator== (const Iterator&) = 0;
    virtual bool operator!= (const Iterator&) = 0;
    virtual ~Iterator() = default;
};

#endif
