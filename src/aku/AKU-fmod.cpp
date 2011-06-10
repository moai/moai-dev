// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-fmod.h>
#include <moaiext-fmod/MOAIFmod.h>
#include <moaiext-fmod/MOAIFmodChannel.h>
#include <moaiext-fmod/MOAIFmodSound.h>

//================================================================//
// AKU-fmod
//================================================================//

//----------------------------------------------------------------//
void AKUFmodInit () {

	MOAIFmod::Get ();
	
	REGISTER_LUA_CLASS ( MOAIFmod )
	REGISTER_LUA_CLASS ( MOAIFmodChannel )
	REGISTER_LUA_CLASS ( MOAIFmodSound )
}

//----------------------------------------------------------------//
void AKUFmodUpdate () {

	MOAIFmod::Get ().Update ();
}