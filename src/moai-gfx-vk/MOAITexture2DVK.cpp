// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/ZLTextureFormat.h>

//================================================================//
// MOAITexture2DVK
//================================================================//

//----------------------------------------------------------------//
MOAITexture2DVK::MOAITexture2DVK () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAITexture2D )
		RTTI_EXTEND ( MOAITextureVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture2DVK::~MOAITexture2DVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAITexture2DVK::MOAIGfxResource_OnCPUCreate () {

	return MOAITexture2D::MOAIGfxResource_OnCPUCreate ();
}

//----------------------------------------------------------------//
void MOAITexture2DVK::MOAIGfxResource_OnCPUPurgeRecoverable () {

	MOAITexture2D::MOAIGfxResource_OnCPUPurgeRecoverable ();
}

//----------------------------------------------------------------//
bool MOAITexture2DVK::MOAIGfxResourceVK_OnGPUCreate () {
	
//	bool success = false;
//
//	if ( this->mImage && this->mImage->IsOK ()) {
//		success =  this->CreateTextureFromImage ( *this->mImage );
//	}
//	else if ( this->mTextureDataFormat && this->mTextureData ) {
//		success = this->mTextureDataFormat->CreateTexture ( *this, this->mTextureData, this->mTextureDataSize );
//	}
//
//	if ( !success ) {
//		this->Destroy ();
//		return false;
//	}
//
//	return this->MOAIGfxResourceVK_OnGPUUpdate ();
	return true;
}

//----------------------------------------------------------------//
void MOAITexture2DVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture2D, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAITexture2DVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture2D, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}
