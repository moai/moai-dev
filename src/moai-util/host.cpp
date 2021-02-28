// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/host.h>
#include <moai-core/headers.h>

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
void AKUUtilContextInitialize ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	context->Affirm < ZLImageFormatMgr >();
	context->Affirm < MOAIMainThreadTaskSubscriber >();

	context->Affirm < MOAIMath >();
	context->Affirm < MOAIFileSystem >();
	context->Affirm < MOAILuaUtil >();
	context->Affirm < MOAIJsonParser >();
	
	// MOAI
	REGISTER_LUA_CLASS ( context, MOAIByteStream )
	REGISTER_LUA_CLASS ( context, MOAIDataBuffer )
	REGISTER_LUA_CLASS ( context, MOAIDataBufferStream )
	REGISTER_LUA_CLASS ( context, MOAIFileStream )
	REGISTER_LUA_CLASS ( context, MOAIFileSystem )
	REGISTER_LUA_CLASS ( context, MOAIFourier )
	
	#ifndef MOAI_WITH_LUAJIT
		REGISTER_LUA_CLASS ( context, MOAILuaUtil )
	#endif

	REGISTER_LUA_CLASS ( context, MOAIHashWriter )
	REGISTER_LUA_CLASS ( context, MOAIMath )
	REGISTER_LUA_CLASS ( context, MOAIMemStream )
	REGISTER_LUA_CLASS ( context, MOAIStreamAdapter )
	REGISTER_LUA_CLASS ( context, MOAITaskQueue )
	
	#if MOAI_WITH_JANSSON
		REGISTER_LUA_CLASS ( context, MOAIJsonParser )
	#endif
	
	#if MOAI_WITH_GPB
		REGISTER_LUA_CLASS ( context, MOAIParser )
	#endif
	
	#if MOAI_WITH_TINYXML && MOAI_WITH_EXPAT
  		REGISTER_LUA_CLASS ( context, MOAIXmlParser )
	#endif
}
