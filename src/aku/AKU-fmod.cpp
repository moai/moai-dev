// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-fmod.h>
#include <moaicore/moaicore.h>

#include <moaicore/MOAIFmod.h>
#include <moaicore/MOAIFmodChannel.h>
#include <moaicore/MOAIFmodSound.h>

//================================================================//
// AKU-fmod
//================================================================//

//----------------------------------------------------------------//
void AKULoadFmod () {

	MOAIFmod::Get ();
	
	REGISTER_LUA_CLASS ( MOAIFmod )
	REGISTER_LUA_CLASS ( MOAIFmodChannel )
	REGISTER_LUA_CLASS ( MOAIFmodSound )
}
