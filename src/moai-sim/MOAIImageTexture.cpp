// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIImageTexture.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	updateRegion
	@text	Update either a sub-region of the texture or the whole
			texture to match changes in the image. Updated regions will
			be reloaded from the image the next time the texture is bound.

	@in		MOAIImageTexture self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIImageTexture::_updateRegion ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImageTexture, "U" )
	
	if ( state.GetTop () > 1 ) {
		ZLIntRect rect = state.GetRect < int >( 2 );
		self->UpdateRegion ( rect );
	}
	else {
		self->UpdateRegion ();
	}
	return 0;
}

//================================================================//
// MOAIImageTexture
//================================================================//

//----------------------------------------------------------------//
void MOAIImageTexture::OnClearDirty () {

	this->mRegion.Clear ();
}

//----------------------------------------------------------------//
MOAIImageTexture::MOAIImageTexture () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAISingleTexture )
		RTTI_EXTEND ( MOAIImage )
	RTTI_END
	
	this->mDebugName = "(stacktrace from MOAIImageTexture)";
}

//----------------------------------------------------------------//
MOAIImageTexture::~MOAIImageTexture () {
}

//----------------------------------------------------------------//
bool MOAIImageTexture::OnGPUCreate () {

	if ( !this->IsOK ()) return false;
	
	this->mRegion.Clear ();
	if ( this->CreateTextureFromImage ( *this )) {
		return this->OnGPUUpdate ();
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageTexture::OnGPUUpdate () {

	bool result = true;

	if ( this->mRegion.Area () > 0 ) {
		result = this->UpdateTextureFromImage ( *this, this->mRegion );
		this->mRegion.Clear ();
	}
	return result && MOAISingleTexture::OnGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAIImageTexture::OnImageStatusChanged	( bool isOK ) {

	if ( isOK ) {
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
void MOAIImageTexture::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAISingleTexture::RegisterLuaClass ( state );
	MOAIImage::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIImageTexture::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAISingleTexture::RegisterLuaFuncs ( state );
	MOAIImage::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "invalidate",					_updateRegion }, // TODO: deprecate
		{ "updateRegion",				_updateRegion },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIImageTexture::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAISingleTexture::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIImageTexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAISingleTexture::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIImageTexture::UpdateRegion () {
	
	this->mRegion = this->GetRect ();
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAIImageTexture::UpdateRegion ( ZLIntRect rect ) {
	
	rect.Bless ();
	this->GetRect ().Clip ( rect );
	
	if ( this->GetState () == STATE_NEEDS_GPU_UPDATE ) {
		this->mRegion.Grow ( rect );
	}
	else {
		this->mRegion = rect;
	}
	
	this->ScheduleForGPUUpdate ();
}
