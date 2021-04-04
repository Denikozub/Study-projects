#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <cmath>

using std::out_of_range;


/*
template class List
linked list, stores index of each element (for sparse matrixes and vectors)
*/

template <class T>
class List {
    struct List_element {
        T data;
        size_t index;
        List_element* next;
        List_element(const T& set_data, size_t set_index,
                List_element* set_next = nullptr);
    };

    typedef List_element* List_p;
    List_p head;
public:
    List() { head = nullptr; }
    List(const List<T>& obj);

    void clear();
    void clone(const List<T>& from);

    // check is an element with index exists
    bool exists(size_t index) const;

    // element to element comparison
    // if T is floating type epsilon is used
    // therefore type T should have operator-
    bool compare(const List<T>& comp, double epsilon = 0.0000001) const;
    T& add(size_t index, const T& data);
    size_t length() const;

    // get data of an element after an element with index
    // if index == -1 return data of first element
    // if element not found or is last return -2
    T& get_next(long int& index) const;
    T& operator[](size_t index) const;
    List<T>& operator=(const List<T>& from);

    ~List() { clear(); }
};


template <typename T>
List<T>::List_element::List_element(const T& set_data, size_t set_index,
        List_element* set_next) {
    data = set_data;
    index = set_index;
    next = set_next;
}

template <typename T>
List<T>::List(const List<T>& obj) {
    head = nullptr; clone(obj);
}

template <typename T>
T& List<T>::operator[](size_t index) const {
    if (head == nullptr)
        throw out_of_range("operator[]: list element not exists");
    List_p temp = head;
    do {
        if (temp->index == index)
            return temp->data;
        temp = temp->next;
    } while (temp != nullptr);
    throw out_of_range("operator[]: list element not exists");
}

template <typename T>
T& List<T>::add(size_t index, const T& data) {
    if (head == nullptr)
        return (head = new List_element(data, index))->data;
    List_p temp = head;
    while (temp->next != nullptr)
        temp = temp->next;
    return (temp->next = new List_element(data, index))->data;
}

template <typename T>
void List<T>::clear() {
    if (head == nullptr)
        return;
    do {
        List_p temp = head->next;
        delete head;
        head = temp;
    } while (head != nullptr);
}

template <typename T>
void List<T>::clone(const List<T>& from) {
    List_p from_head = from.head;
    clear();
    if (from_head == nullptr)
        return;
    head = new List_element(from_head->data, from_head->index);
    List_p save_head = head;
    from_head = from_head->next;
    while (from_head != nullptr) {
        head->next = new List_element(from_head->data, from_head->index);
        from_head = from_head->next, head = head->next;
    }
    head = save_head;
}

template <typename T>
size_t List<T>::length() const {
    size_t count = 0;
    List_p temp = head;
    while (temp != nullptr)
        count++, temp = temp->next;
    return count;
}

template <typename T>
T& List<T>::get_next(long int& index) const {
    if (head == nullptr)
        throw out_of_range("get_next(): list is empty");
    if (index == -1) {
        index = head->index;
        return head->data;
    }
    List_p temp = head;
    while (temp != nullptr && (long int)temp->index != index)
        temp = temp->next;
    if (temp == nullptr || temp->next == nullptr) {
        index = -2;
        return head->data;
    }
    index = temp->next->index;
    return temp->next->data;
}

template <typename T>
bool List<T>::exists(size_t index) const {
    if (head == nullptr)
        return false;
    List_p temp = head;
    do {
        if (temp->index == index)
            return true;
        temp = temp->next;
    } while (temp != nullptr);
    return false;
}

template <typename T>
bool List<T>::compare(const List<T>& comp, double epsilon) const {
    List_p temp1 = comp.head, temp2 = head;
    if (temp2 == nullptr)
        return temp1 == nullptr;
    if (temp1 == nullptr)
        return temp2 == nullptr;
    if (length() != comp.length())
        return false;
    while (temp2 != nullptr) {
        if (temp2->index != temp1->index || temp2->data != temp1->data)
            return false;
        temp2 = temp2->next;
        temp1 = temp1->next;
    }
    return true;
}

template <>
bool List<double>::compare(const List<double>& comp, double epsilon) const {
    List_p temp1 = comp.head, temp2 = head;
    if (temp2 == nullptr)
        return temp1 == nullptr;
    if (temp1 == nullptr)
        return temp2 == nullptr;
    if (length() != comp.length())
        return false;
    while (temp2 != nullptr) {
        if (temp2->index != temp1->index ||
            std::fabs(temp2->data - temp1->data) > epsilon)
            return false;
        temp2 = temp2->next;
        temp1 = temp1->next;
    }
    return true;
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& from) {
    clone(from); return *this;
}

#endif
