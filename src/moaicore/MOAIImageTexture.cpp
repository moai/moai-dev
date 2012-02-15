// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIImageTexture.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	invalidate
	@text	Invalidate either a sub-region or the whole image.

	@in		MOAIImage self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIImageTexture::_invalidate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImageTexture, "U" )
	
	if ( self->mStatus != INVALID ) {
	
		if ( state.GetTop () > 1 ) {
		
			USIntRect rect = state.GetRect < int >( 2 );
			
			rect.Bless ();
			USIntRect imageRect = self->GetRect ();
			imageRect.Clip ( rect );
			
			if ( self->mStatus == VALID ) {
				self->mRegion = rect;
			}
			else {
				self->mRegion.Grow ( rect );
			}
			self->mStatus = INVALID_REGION;
		}
		else {
			self->mStatus = INVALID;
		}
	}
	return 0;
}

//================================================================//
// MOAIImageTexture
//================================================================//

//----------------------------------------------------------------//
bool MOAIImageTexture::IsRenewable () {

	return true;
}

//----------------------------------------------------------------//
bool MOAIImageTexture::IsValid () {

	return ( this->mGLTexID && ( this->mStatus == VALID ));
}

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
void MOAIImageTexture::OnClear () {

	this->MOAIImage::Clear ();
	MOAITextureBase::OnClear ();
}

//----------------------------------------------------------------//
void MOAIImageTexture::OnLoad () {

	if ( !this->IsOK ()) return;
	
	if ( !this->mGLTexID ) {
		this->CreateTextureFromImage ( *this );
	}
	else if ( this->mStatus != VALID ) {
		
		USIntRect rect = this->mRegion;
		if ( this->mStatus == INVALID ) {
			rect = this->GetRect ();
		}
		this->UpdateTextureFromImage ( *this, rect );
	}
	this->mStatus = VALID;
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
		{ "invalidate",				_invalidate },
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
