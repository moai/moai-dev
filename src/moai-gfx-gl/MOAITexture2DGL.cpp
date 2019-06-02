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

	RTTI_BEGIN
		RTTI_EXTEND ( MOAITexture2D )
		RTTI_EXTEND ( MOAITextureGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture2DGL::~MOAITexture2DGL () {
}

//----------------------------------------------------------------//
void MOAITexture2DGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITexture2D::RegisterLuaClass ( state );
	MOAITextureGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITexture2DGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAITexture2D::RegisterLuaFuncs ( state );
	MOAITextureGL::RegisterLuaFuncs ( state );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAITexture2DGL::MOAIGfxResource_OnCPUCreate () {

	return MOAITexture2D::MOAIGfxResource_OnCPUCreate ();
}

//----------------------------------------------------------------//
void MOAITexture2DGL::MOAIGfxResource_OnCPUPurgeRecoverable () {

	MOAITexture2D::MOAIGfxResource_OnCPUPurgeRecoverable ();
}

//----------------------------------------------------------------//
bool MOAITexture2DGL::MOAIGfxResourceGL_OnGPUCreate () {
	
	bool success = false;
	
	if ( this->mImage && this->mImage->IsOK ()) {
		success =  this->CreateTextureFromImage ( *this->mImage );
	}
	else if ( this->mTextureDataFormat && this->mTextureData ) {
		success = this->mTextureDataFormat->CreateTexture ( *this, this->mTextureData, this->mTextureDataSize );
	}
	
	if ( !success ) {
		this->Destroy ();
		return false;
	}
	
	return this->MOAIGfxResourceGL_OnGPUUpdate ();
}
