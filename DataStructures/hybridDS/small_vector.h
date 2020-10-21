#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <cstring>

using std::cout;
using std::endl;

template<typename T>
class SmallVectorImpl
{
    T *m_begin, *m_end;
   size_t m_capacity, m_unused;
protected:
    SmallVectorImpl(T *begin, T *end,
                  size_t capacity) :
        m_begin(begin), m_end(end),
        m_capacity(capacity), m_unused(capacity) {}

public:
    class iterator: public std::iterator<
                        std::input_iterator_tag,   // iterator_category
                        long,                      // value_type
                        long,                      // difference_type
                        const long*,               // pointer
                        long                       // reference
                                      >
    {
        T* ptr = nullptr;
    public:
        explicit iterator(T* _ptr) : ptr(_ptr) {}
        iterator& operator++() { ++ptr; return *this;}
        iterator operator++(int) {iterator retval = *this; ++ptr; return retval;}
        bool operator==(iterator other) const {return ptr == other.ptr;}
        bool operator!=(iterator other) const {return (ptr != other.ptr);}
        reference operator*() const {return *ptr;}
    };


    iterator begin() {return iterator(m_begin);}
    iterator end() {return iterator(m_end);}

    void push_back(const T& element);
    void pop_back(T& element);
    T& operator[](int ind) const { return *(m_begin + ind); }
    T& operator*() const { return *m_begin; }
    void resize(size_t size)
    {
        if (size >= m_capacity) size = m_capacity - 1;
        m_end += size;
    }
    size_t size() const { return (m_capacity - m_unused); }
};

template<typename T>
inline void SmallVectorImpl<T>::push_back(const T &element)
{
    if (m_unused > 0){	// you can also do if ((m_end - m_begin) / sizeof(T) < capacity)
        *(m_end++) = element;
        --m_unused;
    }
}

template<typename T>
inline void SmallVectorImpl<T>::pop_back(T& element)
{
    if (m_end > m_begin){
        element = *(m_end--);
        ++m_unused;
    }
}

template<typename T, int N>
class SmallVector : public SmallVectorImpl<T>
{
    alignas (T) char Buffer[sizeof(T) * N];

public:
    SmallVector() : SmallVectorImpl<T>(reinterpret_cast<T*>(Buffer),
                                       reinterpret_cast<T*>(Buffer),
                                      N)
    {
//        memset(Buffer, 0, N*(sizeof(T)));
    }
    SmallVector(std::initializer_list<T> l) : SmallVectorImpl<T>(reinterpret_cast<T*>(Buffer),
                                       reinterpret_cast<T*>(Buffer+l.size()*sizeof (T)),
                                      N)
    {
        memcpy(Buffer, &(*l.begin()), l.size()*sizeof (T));
    }
    //...
};
