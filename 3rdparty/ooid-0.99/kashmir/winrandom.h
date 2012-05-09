// winrandom.h -- Windows random number generator

// Copyright (C) 2008 Kenneth Laskoski

/** @file winrandom.h
    @brief Windows random number generator
    @author Copyright (C) 2008 Kenneth Laskoski

    with contribution of
    @author Chet Stuut

    Use, modification, and distribution are subject to the
    Boost Software License, Version 1.0. See accompanying file
    LICENSE_1_0.txt or <http://www.boost.org/LICENSE_1_0.txt>.
*/

#ifndef KL_WINRANDOM_H 
#define KL_WINRANDOM_H 

#include "randomstream.h"
#include "unique.h"

#include <stdexcept>

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <wincrypt.h>

namespace kashmir {
namespace system {

class WinRandom : public user::randomstream<WinRandom>, unique<WinRandom>
{
    HCRYPTPROV hProv;

public:
    WinRandom()
    {
        if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))
            if (GetLastError() != NTE_BAD_KEYSET || !CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
                throw std::runtime_error("failed to acquire cryptographic context.");
    }

    ~WinRandom()
    {
        CryptReleaseContext(hProv, 0);
    }

    void read(char* buffer, std::size_t count)
    {
        if (!CryptGenRandom(hProv, count, buffer))
            throw std::runtime_error("system failed to generate random data.");
    }
};

}}

#endif
