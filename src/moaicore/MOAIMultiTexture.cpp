// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIMultiTexture.h>
#include <moaicore/MOAITexture.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAIMultiTexture::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMultiTexture, "U" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	
	self->Reserve ( total );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIMultiTexture::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMultiTexture, "UN" )
	
	u32 idx					= state.GetValue < u32 >( 2, 1 ) - 1;
	MOAITexture* texture	= state.GetLuaObject < MOAITexture >( 3 );
	
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
void MOAIMultiTexture::SetTexture ( u32 idx, MOAITexture* texture ) {

	if ( idx >= this->mTextures.Size ()) return;
	if ( this->mTextures [ idx ] == texture ) return;

	if ( texture ) {
		this->LuaRetain ( *texture );
	}
	
	if ( this->mTextures [ idx ]) {
		this->LuaRelease ( *this->mTextures [ idx ]);
	}
	
	this->mTextures [ idx ] = texture;
}
