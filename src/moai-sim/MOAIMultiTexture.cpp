// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAISingleTexture.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	release
	@text	Releases any memory associated with the texture.
	
	@in		MOAIMultiTexture self
	@out	nil
*/
int MOAIMultiTexture::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMultiTexture, "U" )
	
	for ( size_t i = 0; i < self->mTextures.Size (); ++i ) {
		MOAISingleTexture* texture = self->mTextures [ i ];
		if ( texture ) {
			texture->Destroy ();
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserve
	@text	Reserve or clears indices for textures.
	
	@in		MOAIMultiTexture self
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
/**	@lua	setTexture
	@text	Sets of clears a texture for the given index.
	
	@in		MOAIMultiTexture self
	@in		number index
	@opt	MOAISingleTexture texture		Default value is nil.
	@out	nil
*/
int MOAIMultiTexture::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMultiTexture, "UN" )
	
	u32 idx						= state.GetValue < u32 >( 2, 1 ) - 1;
	MOAISingleTexture* texture	= state.GetLuaObject < MOAISingleTexture >( 3, true );
	
	self->SetTexture ( idx, texture );
	
	return 0;
}

//================================================================//
// MOAIMultiTexture
//================================================================//

//----------------------------------------------------------------//
u32 MOAIMultiTexture::CountActiveUnits () {

	return ( u32 )this->mTextures.Size ();
}

//----------------------------------------------------------------//
MOAISingleTexture* MOAIMultiTexture::GetTextureForUnit ( u32 unit ) {

	assert ( unit < this->mTextures.Size ());
	return this->mTextures [ unit ];
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

	this->mTextures.Init ( total );
	this->mTextures.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIMultiTexture::SetTexture ( u32 idx, MOAISingleTexture* texture ) {

	if ( idx >= this->mTextures.Size ()) return;
	if ( this->mTextures [ idx ] == texture ) return;

	this->LuaRetain ( texture );
	this->LuaRelease ( this->mTextures [ idx ]);
	this->mTextures [ idx ] = texture;
}
