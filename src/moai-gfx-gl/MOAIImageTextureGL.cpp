// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIImageTextureGL.h>

//================================================================//
// MOAIImageTextureGL
//================================================================//

//----------------------------------------------------------------//
MOAIImageTextureGL::MOAIImageTextureGL () {
	
	RTTI_BEGIN ( MOAIImageTextureGL )
		RTTI_EXTEND ( MOAIImageTexture )
		RTTI_EXTEND ( MOAITextureGL )
	RTTI_END
	
	this->mDebugName = "(stacktrace from MOAIImageTextureGL)";
}

//----------------------------------------------------------------//
MOAIImageTextureGL::~MOAIImageTextureGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIImageTextureGL::MOAIGfxResourceGL_OnGPUCreate () {

	if ( !this->IsOK ()) return false;
	
	this->mRegion.Clear ();
	if ( this->CreateTextureFromImage ( *this )) {
		return this->MOAIGfxResourceGL_OnGPUUpdate ();
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageTextureGL::MOAIGfxResourceGL_OnGPUUpdate () {

	bool result = true;

	if ( this->mRegion.Area () > 0 ) {
		result = this->UpdateTextureFromImage ( *this, this->mRegion );
		this->mRegion.Clear ();
	}
	return result && MOAITextureGL::MOAIGfxResourceGL_OnGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIImage, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureGL, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIImage, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureGL, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}
