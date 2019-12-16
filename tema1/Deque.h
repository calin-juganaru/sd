// Copyright [2018] <Călin Jugănaru>

#ifndef DEQUE_H_
#define DEQUE_H_

#include "DoublyLinkedList.h"

template <typename T>
class Deque
{
    DoublyLinkedList<T> _list;

 public:
    Deque() = default;
    ~Deque() = default;

    Deque<T>& operator=(const Deque<T>& other)
    {
        this->_list = other._list;
        return *this;
    }

    void push_front(const T& value)
    {
        _list.push_front(value);
    }

    void push_back(const T& value)
    {
        _list.push_back(value);
    }

    void pop_front()
    {
        _list.pop_front();
    }

    void pop_back()
    {
        _list.pop_back();
    }

    T front()
    {
        return _list.front();
    }

    T back()
    {
        return _list.back();
    }

    size_t size()
    {
        return _list.size();
    }

    bool empty()
    {
        return _list.empty();
    }

    list_iterator<T> begin()
    {
        return _list.begin();
    }

    list_iterator<T> end()
    {
        return _list.end();
    }
};

#endif  // DEQUE_H_
