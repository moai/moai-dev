// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDevice.h>
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
MOAIImageTexture::MOAIImageTexture () :
	mStatus ( INVALID ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITextureBase )
		RTTI_EXTEND ( MOAIImage )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIImageTexture::~MOAIImageTexture () {
}

//----------------------------------------------------------------//
void MOAIImageTexture::OnGPUBind () {

	if ( this->mStatus == INVALID ) {
		this->UpdateTextureFromImage ( *this, this->mRegion );
		this->mStatus = VALID;
	}
	MOAITextureBase::OnGPUBind ();
}

//----------------------------------------------------------------//
bool MOAIImageTexture::OnGPUCreate () {

	if ( !this->IsOK ()) return false;
	this->mStatus = VALID;
	return this->CreateTextureFromImage ( *this );
}

//----------------------------------------------------------------//
void MOAIImageTexture::OnImageStatusChanged	( bool isOK ) {

	if ( isOK ) {
		this->FinishInit ();
		this->DoCPUAffirm ();
	}
}

//----------------------------------------------------------------//
void MOAIImageTexture::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITextureBase::RegisterLuaClass ( state );
	MOAIImage::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIImageTexture::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAITextureBase::RegisterLuaFuncs ( state );
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
	MOAITextureBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIImageTexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAITextureBase::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIImageTexture::UpdateRegion () {
	
	this->mRegion = this->GetRect ();
	this->mStatus = INVALID;
}

//----------------------------------------------------------------//
void MOAIImageTexture::UpdateRegion ( ZLIntRect rect ) {
	
	rect.Bless ();
	this->GetRect ().Clip ( rect );
	
	if ( this->mStatus == VALID ) {
		this->mRegion = rect;
	}
	else {
		this->mRegion.Grow ( rect );
	}
	this->mStatus = INVALID;
}
