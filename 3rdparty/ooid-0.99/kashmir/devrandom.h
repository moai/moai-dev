// devrandom.h -- UNIX random number generator

// Copyright (C) 2008 Kenneth Laskoski

/** @file devrandom.h
    @brief UNIX random number generator
    @author Copyright (C) 2008 Kenneth Laskoski

    Use, modification, and distribution are subject to the
    Boost Software License, Version 1.0. See accompanying file
    LICENSE_1_0.txt or <http://www.boost.org/LICENSE_1_0.txt>.
*/

#ifndef KL_DEVRANDOM_H 
#define KL_DEVRANDOM_H 

#include "randomstream.h"
#include "unique.h"

#include <fstream>
#include <stdexcept>

namespace kashmir {
namespace system {

class DevRandom : public user::randomstream<DevRandom>, unique<DevRandom>
{
    std::ifstream file;

public:
    DevRandom() : file("/dev/random", std::ios::binary)
    {
        if (!file)
		    // MOAI
            //throw std::runtime_error("failed to open random device.");
			assert(false);
    }

    void read(char* buffer, std::size_t count)
    {
        file.read(buffer, count);
    }
};

}}

#endif
