// winrand.h -- Windows random number generator

// Copyright (C) 2008 Kenneth Laskoski

/** @file winrand.h
    @brief Windows random number generator
    @author Copyright (C) 2008 Kenneth Laskoski

    WinRand uses ADVAPI32!RtlGenRandom and does not require a CSP context
    see <http://blogs.msdn.com/michael_howard/archive/2005/01/14/353379.aspx>
    and <http://msdn.microsoft.com/en-us/library/aa387694%28VS.85%29.aspx>

    Use, modification, and distribution are subject to the
    Boost Software License, Version 1.0. See accompanying file
    LICENSE_1_0.txt or <http://www.boost.org/LICENSE_1_0.txt>.
*/

#ifndef KL_WINRAND_H
#define KL_WINRAND_H

#include "randomstream.h"
#include "unique.h"

#include <stdexcept>

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#include <windows.h>

namespace kashmir {
namespace system {

class WinRand : public user::randomstream<WinRand>, unique<WinRand>
{
    HMODULE advapi32;
    BOOLEAN (APIENTRY *RtlGenRandom)(void*, ULONG);

public:
    WinRand() : advapi32(LoadLibrary("ADVAPI32.DLL"))
    {
        if (!advapi32)
            throw std::runtime_error("failed to load ADVAPI32.DLL.");

        RtlGenRandom = reinterpret_cast<BOOLEAN (APIENTRY *)(void*, ULONG)>(GetProcAddress(advapi32, "SystemFunction036"));
        if (!RtlGenRandom)
        {
            FreeLibrary(advapi32);
            throw std::runtime_error("failed to get ADVAPI32!RtlGenRandom address.");
        }
    }

    ~WinRand()
    {
        FreeLibrary(advapi32);
    }

    void read(char* buffer, std::size_t count)
    {
        if (!RtlGenRandom(buffer, count))
            throw std::runtime_error("system failed to generate random data.");
    }
};

}}

#endif
