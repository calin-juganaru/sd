// Copyright [2018] <Călin Jugănaru>

#ifndef DOUBLYLINKEDLIST_H_
#define DOUBLYLINKEDLIST_H_

#include <iostream>

template <typename T>
struct Node
{
    T _value;

    Node<T>* prev;
    Node<T>* next;

    Node():
        prev(nullptr),
        next(nullptr) {}

    explicit Node(const T& value):
        prev(nullptr),
        next(nullptr),
        _value(value) {}

    ~Node() = default;

    Node<T>& operator=(const Node<T>& other) = default;
};

template <typename T>
class list_iterator
{
    Node<T>* pointer;

 public:
    list_iterator():
        pointer(nullptr) {}

    ~list_iterator() = default;

    explicit list_iterator(Node<T>* p):
        pointer(p) {}

    list_iterator& operator++()
    {
        pointer = pointer->next;
        return *this;
    };

    T& operator*()
    {
        return pointer->_value;
    }

    T* operator->()
    {
        return &pointer->_value;
    }

    bool operator!=(const list_iterator<T>& other)
    {
        if (this->pointer)
            return (this->pointer != other.pointer);
        else
            return false;
    }
};

template <typename T>
class DoublyLinkedList
{
    Node<T>* first;
    Node<T>* last;
    size_t _size;

 public:
    DoublyLinkedList():
        first(nullptr),
        last(nullptr),
        _size(0) {}

    ~DoublyLinkedList()
    {
        auto aux = first;
        while (first)
        {
            aux = aux->next;
            delete first;
            first = aux;
        }
    }

    DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& other)
    {
        auto aux = other.first;
        while (aux)
        {
            this->push_back(aux->_value);
            aux = aux->next;
        }
        return *this;
    }

    void push_front(const T& value)
    {
        if (_size == 0)
        {
            first = new Node<T>(value);
            last = first;
            first->prev = nullptr;
            last->next = nullptr;
        }
        else
        {
            first->prev = new Node<T>(value);
            first->prev->next = first;
            first = first->prev;
        }
        ++_size;
    }

    void push_back(const T& value)
    {
        auto aux = new Node<T>(value);
        if (_size == 0)
        {
            first = last = aux;
            first->prev = nullptr;
            last->next = nullptr;
        }
        else
        {
            aux->prev = last;
            last->next = aux;
            last = aux;
        }
        ++_size;
    }

    void insert(size_t pos, T value);
    void erase(size_t pos);

    void pop_front()
    {
        if (!_size) return;
        else if (_size == 1)
        {
            delete first;
            first = last = nullptr;
        }
        else
        {
            auto aux = first->next;
            delete first;
            first = aux;
            first->prev = nullptr;
        }
        --_size;
    }

    void pop_back()
    {
        if (!_size) return;
        else if (_size == 1)
        {
            delete last;
            first = last = nullptr;
        }
        else
        {
            auto aux = last->prev;
            delete last;
            last = aux;
            last->next = nullptr;
        }
        --_size;
    }

    T front()
    {
        return first->_value;
    }

    T back()
    {
        return last->_value;
    }

    size_t size()
    {
        return _size;
    }

    bool empty()
    {
        return !_size;
    }

    typedef list_iterator<T> iterator_t;

    iterator_t begin()
    {
        return iterator_t(first);
    }

    iterator_t end()
    {
        if (_size)
            return iterator_t(last->next);
        else
            return iterator_t();
    }
};

#endif  // DOUBLYLINKEDLIST_H_
