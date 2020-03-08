// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAITexture2DGL.h>
#include <moai-gfx-gl/ZLTextureFormat.h>

//================================================================//
// MOAITexture2DGL
//================================================================//

//----------------------------------------------------------------//
MOAITexture2DGL::MOAITexture2DGL () {

	RTTI_BEGIN ( MOAITexture2DGL )
		RTTI_EXTEND ( MOAITexture2D )
		RTTI_EXTEND ( MOAITextureGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture2DGL::~MOAITexture2DGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAITexture2DGL::MOAIGfxResourceGL_OnGPUCreate () {
	
	bool success = false;
	
	if ( this->mImage && this->mImage->IsOK ()) {
		success = this->CreateTextureFromImage ( *this->mImage );
	}
	else if ( this->mTextureDataFormat && this->mTextureData ) {
		success = this->mTextureDataFormat->CreateTexture ( *this, this->mTextureData, this->mTextureDataSize );
	}
	
	if ( !success ) {
		this->Finalize ();
		return false;
	}
	
	return this->MOAIGfxResourceGL_OnGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAITexture2DGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture2D, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureGL, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAITexture2DGL::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture2D, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureGL, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}
