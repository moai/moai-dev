// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIImageTextureVK.h>

//================================================================//
// MOAIImageTextureVK
//================================================================//

//----------------------------------------------------------------//
MOAIImageTextureVK::MOAIImageTextureVK () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIImageTexture )
		RTTI_EXTEND ( MOAITextureVK )
	RTTI_END
	
//	this->mDebugName = "(stacktrace from MOAIImageTextureVK)";
}

//----------------------------------------------------------------//
MOAIImageTextureVK::~MOAIImageTextureVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIImageTextureVK::MOAIGfxResourceVK_OnGPUCreate () {

//	if ( !this->IsOK ()) return false;
//
//	this->mRegion.Clear ();
//	if ( this->CreateTextureFromImage ( *this )) {
//		return this->MOAIGfxResourceVK_OnGPUUpdate ();
//	}
//	return false;
	return true;
}

//----------------------------------------------------------------//
bool MOAIImageTextureVK::MOAIGfxResourceVK_OnGPUUpdate () {

//	bool result = true;
//
//	if ( this->mRegion.Area () > 0 ) {
//		result = this->UpdateTextureFromImage ( *this, this->mRegion );
//		this->mRegion.Clear ();
//	}
//	return result && MOAITextureVK::MOAIGfxResourceVK_OnGPUUpdate ();
	return true;
}

//----------------------------------------------------------------//
void MOAIImageTextureVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIImage, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIImageTextureVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIImage, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}
