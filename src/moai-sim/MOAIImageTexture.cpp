// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
MOAIImageTexture::MOAIImageTexture () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAbstractGfxResource )
		RTTI_EXTEND ( MOAIImage )
		RTTI_EXTEND ( MOAITextureBase )
	RTTI_END
	
	this->mDebugName = "(stacktrace from MOAIImageTexture)";
}

//----------------------------------------------------------------//
MOAIImageTexture::~MOAIImageTexture () {
}

//----------------------------------------------------------------//
void MOAIImageTexture::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIGfxResource < ZLTexture >::RegisterLuaClass ( state );
	MOAIImage::RegisterLuaClass ( state );
	MOAITextureBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIImageTexture::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource < ZLTexture >::RegisterLuaFuncs ( state );
	MOAIImage::RegisterLuaFuncs ( state );
	MOAITextureBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "invalidate",					_updateRegion }, // TODO: deprecate
		{ "updateRegion",				_updateRegion },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIImageTexture::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIImage::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIImageTexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIImage::SerializeOut ( state, serializer );
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

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIImageTexture::MOAIImage_OnImageStatusChanged	( bool isOK ) {

	if ( isOK ) {
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
void MOAIImageTexture::ZLAbstractGfxResource_OnClearDirty () {

	this->mRegion.Clear ();
}

//----------------------------------------------------------------//
bool MOAIImageTexture::ZLAbstractGfxResource_OnGPUCreate () {

	if ( !this->IsOK ()) return false;
	
	this->mRegion.Clear ();
	if ( this->CreateTextureFromImage ( *this )) {
		return this->ZLAbstractGfxResource_OnGPUUpdate ();
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageTexture::ZLAbstractGfxResource_OnGPUUpdate () {

	bool result = true;

	if ( this->mRegion.Area () > 0 ) {
		result = this->UpdateTextureFromImage ( *this, this->mRegion );
		this->mRegion.Clear ();
	}
	return result && MOAITextureBase::ZLAbstractGfxResource_OnGPUUpdate ();
}
