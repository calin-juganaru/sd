// Copyright [2018] <Călin Jugănaru>

#ifndef STACK_H_
#define STACK_H_

#include "ResizableArray.h"

template <typename T>
class Stack
{
    ResizableArray<T> _array;

 public:
    Stack() = default;
    ~Stack() = default;
    Stack<T>& operator=(const Stack& other) = default;

    void push(const T& value)
    {
        _array.push_back(value);
    }

    void pop()
    {
        _array.pop_back();
    }

    T top()
    {
        return _array.back();
    }

    size_t size()
    {
        return _array.size();
    }

    bool empty()
    {
        return _array.empty();
    }
};

#endif  // STACK_H_
