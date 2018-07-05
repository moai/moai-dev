// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAILight.h>
#include <moai-sim/MOAILightFormat.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAITexture.h>

//================================================================//
// local
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

	u32 uniformID	= state.GetValue < u32 >( 2, 1 ) - 1;
	self->SetUniform ( L, 3, self->mBuffer, uniformID, 0 );
	return 0;
}

//================================================================//
// MOAILight
//================================================================//

//----------------------------------------------------------------//
void MOAILight::ApplyUniforms ( void* buffer, size_t bufferSize ) {

	size_t srcSize = this->mBuffer.Size ();
	if ( srcSize < bufferSize ) {
		memcpy ( buffer, this->mBuffer, srcSize );
	}
}

//----------------------------------------------------------------//
void MOAILight::BindTextures ( u32 textureOffset ) {

	MOAIGfxMgr& gfx = MOAIGfxMgr::Get ();

	size_t nTextures = this->mTextures.Size ();
	for ( u32 i = 0; i < nTextures; ++i ) {
		gfx.mGfxState.SetTexture ( this->mTextures [ i ], textureOffset + i );
	}
}

//----------------------------------------------------------------//
MOAILight::MOAILight () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAILight::~MOAILight () {

	this->mFormat.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAILight::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAILight::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

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
void MOAILight::SetFormat ( MOAILightFormat* format ) {

	this->mFormat.Set ( *this, format );
	if ( format ) {
		format->Bless ();
		this->mBuffer.Init ( format->mBufferSize );
	}
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAILight::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	return this->mFormat ? this->MOAIShaderUniformSchema::ApplyAttrOp ( this->mBuffer, attrID, attr, op ) : false;
}

//----------------------------------------------------------------//
MOAIShaderUniformHandle MOAILight::MOAIShaderUniformSchema_GetUniformHandle ( void* buffer, u32 uniformID ) const {

	MOAIShaderUniformHandle uniform;
	uniform.mBuffer = 0;

	if ( this->mFormat ) {
	
		const MOAILightFormatUniform& lightUniform = this->mFormat->mUniforms [ uniformID ];

		uniform.mType		= lightUniform.mType;
		uniform.mWidth		= lightUniform.mWidth;
		uniform.mBuffer		= ( void* )(( size_t )this->mBuffer.Data () + lightUniform.mBase );
	}
	return uniform;
}

