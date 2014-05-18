# Copyright (C) 2007-2011 LuaDist.
# Created by Peter Kapec
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

project ( luacrypto C )
cmake_minimum_required ( VERSION 2.6 )
include ( dist.cmake )

# Find libraries
find_path ( OPENSSL_INCLUDE_DIR NAMES openssl/err.h )
include_directories ( ${OPENSSL_INCLUDE_DIR} )
find_library ( CRYPTO_LIBRARY NAMES crypto )
find_library ( SSL_LIBRARY NAMES ssl )
if ( WIN32 )
	set ( LIBS gdi32 )
endif ()
include_directories ( src )

if ( WIN32 )
  add_definitions ( "-DLUACRYPTO_API=__declspec(dllexport)" )
endif ( WIN32 )

option( DONT_USE_OPENSSL_DEPRECATED_FUNCTIONS
	"Prevent the use of deprecated public/private key generation functions" ON)
if ( DONT_USE_OPENSSL_DEPRECATED_FUNCTIONS )
	add_definitions ( -DDONT_USE_OPENSSL_DEPRECATED_FUNCTIONS )
endif ( DONT_USE_OPENSSL_DEPRECATED_FUNCTIONS )

install_lua_module ( crypto src/lcrypto.c )
target_link_libraries ( crypto ${CRYPTO_LIBRARY} ${SSL_LIBRARY} ${LIBS})

install_data ( README )
install_doc ( doc/ )
install_test ( tests/ )

add_lua_test ( tests/test.lua )  #FIX: arg[1]
add_lua_test ( tests/rand.lua )
add_lua_test ( tests/encrypt.lua )
add_lua_test ( tests/pkeytest.lua )
add_lua_test ( tests/open_seal.lua )
