// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef _WIN32

#include <uslscore/USUnique.h>

#pragma warning ( disable:4512 )
#pragma warning ( disable:4355 )

#include <kashmir/winrand.h>
#include <kashmir/uuid.h>

//================================================================//
// USUnique
//================================================================//

//----------------------------------------------------------------//
STLString USUnique::GetGUID () {
	
	kashmir::system::WinRand winrandom;
	std::stringstream buffer;
	
	kashmir::uuid_t uuid;
    
	winrandom >> uuid;
    buffer << uuid;

	return buffer.str ();
}

#endif
