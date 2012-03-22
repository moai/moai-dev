// array.h -- STL container interface to arrays

// Copyright (C) 2009 Kenneth Laskoski

/** @file array.h
    @brief STL container interface to arrays
    @author Copyright (C) 2009 Kenneth Laskoski

    Use, modification, and distribution are subject to the
    Boost Software License, Version 1.0. See accompanying file
    LICENSE_1_0.txt or <http://www.boost.org/LICENSE_1_0.txt>.
*/

#ifndef KL_ARRAY_H
#define KL_ARRAY_H

#include <cstddef>
#include <algorithm>

namespace kashmir {

template<class T, std::size_t N>
class array
{
    T data[N];

public:
    array() {}
    ~array() {}

    typedef T value_type;
    typedef std::size_t size_type;

    enum { size = N };

    // copy and assignment
    array(const array& rhs)
    {
        std::copy(rhs.data, rhs.data+size, data);
    }

    array& operator=(const array& rhs)
    {
        std::copy(rhs.data, rhs.data+size, data);
        return *this;
    }

    value_type& operator[](size_type i) { return data[i]; }
    const value_type& operator[](size_type i) const { return data[i]; }

    typedef value_type* iterator;
    iterator begin() { return data; }
    iterator end() { return data+N; }

    typedef const value_type* const_iterator;
    const_iterator begin() const { return data; }
    const_iterator end() const { return data+N; }

    typedef std::reverse_iterator<iterator> reverse_iterator;
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }

    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
};

// comparison operators define a total order
template<class T, std::size_t N>
inline bool operator==(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T, std::size_t N>
inline bool operator<(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, std::size_t N> inline bool operator>(const array<T, N>& lhs, const array<T, N>& rhs) { return (rhs < lhs); }
template<class T, std::size_t N> inline bool operator<=(const array<T, N>& lhs, const array<T, N>& rhs) { return !(rhs < lhs); }
template<class T, std::size_t N> inline bool operator>=(const array<T, N>& lhs, const array<T, N>& rhs) { return !(lhs < rhs); }
template<class T, std::size_t N> inline bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs) { return !(lhs == rhs); }

} // namespace kashmir

#endif
