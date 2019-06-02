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
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIImageTexture )
		RTTI_EXTEND ( MOAITextureGL )
	RTTI_END
	
	this->mDebugName = "(stacktrace from MOAIImageTextureGL)";
}

//----------------------------------------------------------------//
MOAIImageTextureGL::~MOAIImageTextureGL () {
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIImage::RegisterLuaClass ( state );
	MOAITextureGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIImageTextureGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIImage::RegisterLuaFuncs ( state );
	MOAITextureGL::RegisterLuaFuncs ( state );
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
