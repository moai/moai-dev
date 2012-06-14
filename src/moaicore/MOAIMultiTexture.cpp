// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIMultiTexture.h>
#include <moaicore/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserve
	@text	Reserve or clears indices for textures.
	
	@in		MOAITextureBase self
	@opt	number total			Default value is 0.
	@out	nil
*/
int MOAIMultiTexture::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMultiTexture, "U" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	
	self->Reserve ( total );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTexture
	@text	Sets of clears a texture for the given index.
	
	@in		MOAITextureBase self
	@in		number index
	@opt	MOAITextureBase texture		Default value is nil.
	@out	nil
*/
int MOAIMultiTexture::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMultiTexture, "UN" )
	
	u32 idx						= state.GetValue < u32 >( 2, 1 ) - 1;
	MOAITextureBase* texture	= state.GetLuaObject < MOAITextureBase >( 3, true );
	
	self->SetTexture ( idx, texture );
	
	return 0;
}

//================================================================//
// MOAIMultiTexture
//================================================================//

//----------------------------------------------------------------//
void MOAIMultiTexture::Clear () {
}

//----------------------------------------------------------------//
bool MOAIMultiTexture::LoadGfxState () {

	return MOAIGfxDevice::Get ().SetTexture ( this );
}

//----------------------------------------------------------------//
MOAIMultiTexture::MOAIMultiTexture () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIMultiTexture::~MOAIMultiTexture () {
}

//----------------------------------------------------------------//
void MOAIMultiTexture::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMultiTexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMultiTexture::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIMultiTexture::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "reserve",				_reserve },
		{ "setTexture",				_setTexture },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMultiTexture::Reserve ( u32 total ) {

	this->Clear ();
	this->mTextures.Init ( total );
	this->mTextures.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIMultiTexture::SetTexture ( u32 idx, MOAITextureBase* texture ) {

	if ( idx >= this->mTextures.Size ()) return;
	if ( this->mTextures [ idx ] == texture ) return;

	this->LuaRetain ( texture );
	this->LuaRelease ( this->mTextures [ idx ]);
	this->mTextures [ idx ] = texture;
}
