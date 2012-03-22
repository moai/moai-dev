// uuid.h -- universally unique ID - as defined by ISO/IEC 9834-8:2005

// Copyright (C) 2008 Kenneth Laskoski

/** @file uuid.h
    @brief universally unique ID - as defined by ISO/IEC 9834-8:2005
    @author Copyright (C) 2008 Kenneth Laskoski

    Use, modification, and distribution are subject to the
    Boost Software License, Version 1.0. See accompanying file
    LICENSE_1_0.txt or <http://www.boost.org/LICENSE_1_0.txt>.
*/

#ifndef KL_UUID_H
#define KL_UUID_H 

#include "array.h"

#include <istream>
#include <ostream>
#include <sstream>
#include "randomstream.h"

#include <cstddef>

#include <stdexcept>
#include <algorithm>

#include "iostate.h"

namespace kashmir {
namespace uuid {

/** @class uuid_t
    @brief This class provides a C++ binding to the UUID type defined in
    - ISO/IEC 9834-8:2005 | ITU-T Rec. X.667 - available at http://www.itu.int/ITU-T/studygroups/com17/oid.html
    - IETF RFC 4122 - available at http://tools.ietf.org/html/rfc4122

    These technically equivalent standards document the code below.
*/

class uuid_t
{
    // an UUID is a string of 16 octets (128 bits)
    // we use an unpacked representation, value_type may be larger than 8 bits,
    // in which case every input operation must assert data[i] < 256 for i < 16
    // note even char may be more than 8 bits in some particular platform
    typedef unsigned char value_type;
    typedef std::size_t size_type;

    enum { size = 16, string_size = 36 };

    typedef array<value_type, size> data_type;
    data_type data;

public:
    // uninitialized memory
    uuid_t() {}
    ~uuid_t() {}

    // copy and assignment
    uuid_t(const uuid_t& rhs) : data(rhs.data) {}

    uuid_t& operator=(const uuid_t& rhs)
    {
        data = rhs.data;
        return *this;
    }

    // initialization from C string
    explicit uuid_t(const char* string)
    {
        std::stringstream stream(string);
        get(stream);
    }

    // test for nil value
    bool is_nil() const
    {
        for (size_type i = 0; i < size; ++i)
            if (data[i])
                return false;
        return true;
    }

    // safe bool idiom
    typedef data_type uuid_t::*bool_type; 

    operator bool_type() const
    {
        return is_nil() ? 0 : &uuid_t::data;
    }

    friend bool operator==(const uuid_t& lhs, const uuid_t& rhs);
    friend bool operator<(const uuid_t& lhs, const uuid_t& rhs);

    // stream operators
    template<class char_t, class char_traits>
    std::basic_ostream<char_t, char_traits>& put(std::basic_ostream<char_t, char_traits>& os) const;

    template<class char_t, class char_traits>
    std::basic_istream<char_t, char_traits>& get(std::basic_istream<char_t, char_traits>& is);

    // version 4 uuid extraction from a random stream
    template<class user_impl>
    user::randomstream<user_impl>& get(user::randomstream<user_impl>& is);
};

// comparison operators define a total order
inline bool operator==(const uuid_t& lhs, const uuid_t& rhs)
{
    return lhs.data == rhs.data;
}

inline bool operator<(const uuid_t& lhs, const uuid_t& rhs)
{
    return lhs.data < rhs.data;
}

inline bool operator>(const uuid_t& lhs, const uuid_t& rhs) { return (rhs < lhs); }
inline bool operator<=(const uuid_t& lhs, const uuid_t& rhs) { return !(rhs < lhs); }
inline bool operator>=(const uuid_t& lhs, const uuid_t& rhs) { return !(lhs < rhs); }
inline bool operator!=(const uuid_t& lhs, const uuid_t& rhs) { return !(lhs == rhs); }

template<class char_t, class char_traits>
std::basic_ostream<char_t, char_traits>& uuid_t::put(std::basic_ostream<char_t, char_traits>& os) const
{
    if (!os.good())
        return os;

    const typename std::basic_ostream<char_t, char_traits>::sentry ok(os);
    if (ok)
    {
        ios_flags_saver flags(os);
        basic_ios_fill_saver<char_t, char_traits> fill(os);

        const std::streamsize width = os.width(0);
        const std::streamsize mysize = string_size;

        // right padding
        if (flags.value() & (std::ios_base::right | std::ios_base::internal))
            for (std::streamsize i = width; i > mysize; --i)
                os << fill.value();

        os << std::hex;
        os.fill(os.widen('0'));

        for (size_t i = 0; i < 16; ++i)
        {
            os.width(2);
            os << static_cast<unsigned>(data[i]);
            if (i == 3 || i == 5 || i == 7 || i == 9)
                os << os.widen('-');
        }

        // left padding
        if (flags.value() & std::ios_base::left)
            for (std::streamsize i = width; i > mysize; --i)
                os << fill.value();
    }

    return os;
}

template<class char_t, class char_traits>
std::basic_istream<char_t, char_traits>& uuid_t::get(std::basic_istream<char_t, char_traits>& is)
{
    if (!is.good())
        return is;

    const typename std::basic_istream<char_t, char_traits>::sentry ok(is);
    if (ok)
    {
        char_t hexdigits[16];
        char_t* const npos = hexdigits+16;

        typedef std::ctype<char_t> facet_t;
        const facet_t& facet = std::use_facet<facet_t>(is.getloc());

        const char* tmp = "0123456789abcdef";
        facet.widen(tmp, tmp+16, hexdigits);

        char_t c;
        char_t* f;
        for (size_t i = 0; i < size; ++i)
        {
            is >> c;
            c = facet.tolower(c);

            f = std::find(hexdigits, npos, c);
            if (f == npos)
            {
                is.setstate(std::ios_base::failbit);
                break;
            }

            data[i] = static_cast<value_type>(std::distance(hexdigits, f));

            is >> c;
            c = facet.tolower(c);

            f = std::find(hexdigits, npos, c);
            if (f == npos)
            {
                is.setstate(std::ios_base::failbit);
                break;
            }

            data[i] <<= 4;
            data[i] |= static_cast<value_type>(std::distance(hexdigits, f));

            if (i == 3 || i == 5 || i == 7 || i == 9)
            {
                is >> c;
                if (c != is.widen('-'))
                {
                    is.setstate(std::ios_base::failbit);
                    break;
                }
            }
        }

        if (!is)
			// MOAI
            //throw std::runtime_error("failed to extract valid uuid from stream.");
			assert(false);
    }

    return is;
}

template<class user_impl>
user::randomstream<user_impl>& uuid_t::get(user::randomstream<user_impl>& is)
{
    // get random bytes

    // RAE: we take advantage of our representation
    is.read(reinterpret_cast<char*>(this), size);

    // a more general solution would be
//    input_iterator<is> it;
//    std::copy(it, it+size, data.begin());

    // if uuid_t::value_type is larger than 8 bits, we need
    // to maintain the invariant data[i] < 256 for i < 16
    // Example (this may impact randomness):
//    for (size_t i = 0; i < size; ++i)
//        data[i] &= 0xff;

    // set variant
    // should be 0b10xxxxxx
    data[8] &= 0xbf;   // 0b10111111
    data[8] |= 0x80;   // 0b10000000

    // set version
    // should be 0b0100xxxx
    data[6] &= 0x4f;   // 0b01001111
    data[6] |= 0x40;   // 0b01000000

    return is;
}

template<class char_t, class char_traits>
inline std::basic_ostream<char_t, char_traits>& operator<<(std::basic_ostream<char_t, char_traits>& os, const uuid_t& uuid)
{
    return uuid.put(os);
}

template<class char_t, class char_traits>
inline std::basic_istream<char_t, char_traits>& operator>>(std::basic_istream<char_t, char_traits>& is, uuid_t& uuid)
{
    return uuid.get(is);
}

template<class user_impl>
inline user::randomstream<user_impl>& operator>>(user::randomstream<user_impl>& is, uuid_t& uuid)
{
    return uuid.get(is);
}

} // namespace kashmir::uuid

using uuid::uuid_t;

} // namespace kashmir

#endif
