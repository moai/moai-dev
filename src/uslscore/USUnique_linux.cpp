// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifndef _WIN32

#include <uslscore/USUnique.h>
#ifdef HAVE_UUID_H
#include <uuid/uuid.h>
#else
#error Please install the development headers and static library for libuuid on this system!
#endif

//================================================================//
// USUnique
//================================================================//

//----------------------------------------------------------------//
STLString USUnique::GetGUID () {
#ifdef HAVE_UUID_H
	uuid_t out;
	char str[37];
	uuid_generate_random (out);
	uuid_unparse (out, &str[0]);
	std::stringstream buffer;
	buffer << &str[0];
	return buffer.str ();
#else
	return std::string();
#endif
}

#endif
