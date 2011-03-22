// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAISerializer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAISerializer
//================================================================//

//----------------------------------------------------------------//
cc8* MOAISerializer::GetFileMagic () {

	return "--MOAI";
}

//----------------------------------------------------------------//
MOAISerializer::MOAISerializer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaSerializer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISerializer::~MOAISerializer () {
}

//----------------------------------------------------------------//
void MOAISerializer::RegisterLuaClass ( USLuaState& state ) {

	USLuaSerializer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAISerializer::RegisterLuaFuncs ( USLuaState& state ) {
	
	USLuaSerializer::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
STLString MOAISerializer::ToString () {

	STLString repr;
	
	return repr;
}
