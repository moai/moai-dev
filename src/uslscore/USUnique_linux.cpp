// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifndef _WIN32

#include <uslscore/USUnique.h>

#include <kashmir/devrand.h>
#include <kashmir/uuid.h>

//================================================================//
// USUnique
//================================================================//

//----------------------------------------------------------------//
STLString USUnique::GetGUID () {
	kashmir::system::DevRand devrandom;
	std::stringstream buffer;

	kashmir::uuid_t uuid;

	devrandom >> uuid;
	buffer << uuid;

	return buffer.str ();
}

#endif
