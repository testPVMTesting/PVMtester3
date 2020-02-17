#ifndef ARRAY_H
#define ARRAY_H

#include <memory>
#include <vector>
#include <assert.h>
#include "logging.h"


template<typename T>
class array
{
private:
    std::shared_ptr<T> m_data;
    size_t m_start;
    size_t m_size;

    array(std::shared_ptr<T> data, size_t start, size_t size)
        : m_data(data)
        , m_start(start)
        , m_size(size) { }

public:
    array(size_t size)
        : m_data(new T[size], std::default_delete<T[]>())
        , m_start(0)
        , m_size(size) { }

    array(const array<T>& arr)
        : m_data(arr.m_data)
        , m_start(arr.m_start)
        , m_size(arr.m_size) { }

    T* ptr() { return m_data.get() + m_start; }
    const T* ptr() const { return m_data.get() + m_start; }

    T& operator [](size_t index)
    {
        CHECK(is_valid_index(index)) << "Index out of range";
         
        return m_data.get()[index + m_start];
    }

    const T& operator [](size_t index) const
    {
        CHECK(is_valid_index(index)) << "Index out of range";

        return m_data.get()[index + m_start];
    }

    size_t size() const
    {
        return m_size;
    }

    bool is_valid_index(size_t index) const
    {
        return index < m_size;
    }

    array<T> slice(size_t start, size_t size)
    {
        assert(is_valid_index(start));
        assert(is_valid_index(start + size - 1));

        return array(m_data, m_start + start, size);
    }

    array<T> slice(size_t start)
    {
        return slice(start, m_size - start);
    }
};

template<typename T>
T* reinterpret(array<uint8_t> buffer)
{
    CHECK(buffer.size() >= sizeof(T)) << "Buffer too small";
    
    return reinterpret_cast<T*>(buffer.ptr());
}

#endif
