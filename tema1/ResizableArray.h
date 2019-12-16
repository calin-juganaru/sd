// Copyright [2018] <Călin Jugănaru>

#ifndef RESIZABLEARRAY_H_
#define RESIZABLEARRAY_H_

#include <cstring>

template <typename T>
class ResizableArray
{
    T* _array;
    size_t _size;
    size_t _capacity;

 public:
    ResizableArray():
        _array(new T[1]),
        _capacity(1),
        _size(0) {}

    explicit ResizableArray(size_t nr):
        _array(new T[nr]),
        _capacity(nr),
        _size(0) {}

    ResizableArray<T>& operator=(const ResizableArray<T>& other)
        = default;

    ~ResizableArray()
    {
        delete[] _array;
    }

    size_t size()
    {
        return _size;
    }

    size_t capacity()
    {
        return _capacity;
    }

    void push_back(const T& value)
    {
        if (_size == _capacity)
        {
            _capacity *= 2;
            T* aux_ptr = new T[_capacity];
            memmove(aux_ptr, _array, _size * sizeof(T));
            delete[] _array;
            _array = aux_ptr;
        }

        _array[_size++] = value;
    }

    void pop_back()
    {
        if (_size)
            --_size;
    }

    T back()
    {
        if (_size)
            return _array[_size - 1];
        else
            return T();
    }

    bool empty()
    {
        return !_size;
    }
};

#endif  // RESIZABLEARRAY_H_
