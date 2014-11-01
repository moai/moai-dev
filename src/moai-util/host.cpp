// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/host.h>
#include <moai-util/headers.h>

//================================================================//
// aku-util
//================================================================//

//----------------------------------------------------------------//
void AKUUtilAppFinalize () {
}

//----------------------------------------------------------------//
void AKUUtilAppInitialize () {
}

//----------------------------------------------------------------//
void AKUUtilContextInitialize () {

	MOAIMainThreadTaskSubscriber::Affirm ();
	
	MOAIMath::Affirm ();
	
	// MOAI
	REGISTER_LUA_CLASS ( MOAIByteStream )
	REGISTER_LUA_CLASS ( MOAIDataBuffer )
	REGISTER_LUA_CLASS ( MOAIDataBufferStream )
	REGISTER_LUA_CLASS ( MOAIDataIOTask )
	REGISTER_LUA_CLASS ( MOAIFileStream )
	REGISTER_LUA_CLASS ( MOAIFileSystem )
	REGISTER_LUA_CLASS ( MOAIHashWriter )
	REGISTER_LUA_CLASS ( MOAIMath )
	REGISTER_LUA_CLASS ( MOAIMemStream )
	REGISTER_LUA_CLASS ( MOAIStreamAdapter )
	REGISTER_LUA_CLASS ( MOAITaskQueue )
	REGISTER_LUA_CLASS ( MOAITaskThread )
	
	#if MOAI_WITH_JANSSON
		REGISTER_LUA_CLASS ( MOAIJsonParser )
	#endif
	
	#if MOAI_WITH_GPB
		REGISTER_LUA_CLASS ( MOAIParser )
	#endif
	
	#if MOAI_WITH_TINYXML && MOAI_WITH_EXPAT
  		REGISTER_LUA_CLASS ( MOAIXmlParser )
	#endif
}
