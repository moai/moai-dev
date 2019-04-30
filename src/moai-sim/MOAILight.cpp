// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAILight.h>
#include <moai-sim/MOAILightFormat.h>

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

	ZLIndex uniformID = state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	self->SetUniform ( L, 3, self->mBuffer.GetBuffer (), uniformID, ZLIndexOp::ZERO );
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

	MOAIGfxMgrGL& gfx = MOAIGfxMgrGL::Get ();

	size_t nTextures = this->mTextures.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nTextures; ++i ) {
		gfx.SetTexture ( this->mTextures [ i ], i + ( ZLSize )textureOffset );
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
bool MOAILight::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	return this->mFormat ? this->MOAIShaderUniformSchemaBase::ApplyAttrOp ( this->mBuffer.GetBuffer (), attrID, attr, op ) : false;
}

//----------------------------------------------------------------//
ZLShaderUniformHandle MOAILight::ZLAbstractShaderUniformSchema_GetUniformHandle ( void* buffer, ZLIndex uniformID ) const {

	ZLShaderUniformHandle uniform;
	uniform.mBuffer = 0;

	if ( this->mFormat ) {
	
		const MOAILightFormatUniform& lightUniform = this->mFormat->mUniforms [ uniformID ];

		uniform.mType		= lightUniform.mType;
		uniform.mWidth		= lightUniform.mWidth;
		uniform.mBuffer		= ( void* )(( size_t )this->mBuffer.GetBuffer () + lightUniform.mBase );
	}
	return uniform;
}
