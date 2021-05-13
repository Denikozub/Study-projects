#ifndef ITERATOR_HPP
#define ITERATOR_HPP

class Iterator {
    virtual void operator++() = 0;
    virtual bool is_end() = 0;
};

#endif
