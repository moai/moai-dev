// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAILightFormat.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILight::_getFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILight, "U" )
	self->mFormat.PushRef ( state );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILight::_setFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILight, "U" )
	self->mFormat.Set ( *self, state.GetLuaObject < MOAILightFormat >( 2, true ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILight::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILight, "U" )
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILight::_setTransform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILight, "U" )
	return 0;
}

//----------------------------------------------------------------//
int MOAILight::_setUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILight, "U" )

	ZLIndex uniformID = state.GetValue < MOAILuaIndex >( 2, 0 );
	self->SetUniformValue ( L, 3, self->mBuffer.GetBuffer (), uniformID, 0 );
	return 0;
}

//================================================================//
// MOAILight
//================================================================//

//----------------------------------------------------------------//
void MOAILight::ApplyUniforms ( void* buffer, size_t bufferSize ) {

	size_t srcSize = this->mBuffer.Size ();
	if ( srcSize < bufferSize ) {
		memcpy ( buffer, this->mBuffer.GetBuffer (), srcSize );
	}
}

//----------------------------------------------------------------//
void MOAILight::BindTextures ( u32 textureOffset ) {

	MOAIGfxMgr& gfx = MOAIGfxMgr::Get ();

	size_t nTextures = this->mTextures.Size ();
	for ( ZLIndex i = 0; i < nTextures; ++i ) {
		gfx.SetTexture ( this->mTextures [ i ], i + ( ZLSize )textureOffset );
	}
}

//----------------------------------------------------------------//
MOAILight::MOAILight () {
	
	RTTI_BEGIN ( MOAILight )
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAILight::~MOAILight () {

	this->mFormat.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAILight::SetFormat ( MOAILightFormat* format ) {

	this->mFormat.Set ( *this, format );
	if ( format ) {
		format->Bless ();
		this->mBuffer.Init ( format->mBufferSize );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAILight::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAILight::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "getFormat",				_getFormat },
		{ "setFormat",				_setFormat },
		{ "setTexture",				_setTexture },
		{ "setTransform",			_setTransform },
		{ "setUniform",				_setUniform },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAILight::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	return this->mFormat ? this->MOAIUniformSchema::ApplyAttrOp ( this->mBuffer.GetBuffer (), attrID, attr, op ) : false;
}
