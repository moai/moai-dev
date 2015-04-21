// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIMaterialBatch.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// MOAIMaterial
//================================================================//

//----------------------------------------------------------------//
MOAIMaterial::MOAIMaterial () :
	mShader ( 0 ),
	mTexture ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" );
	state.Push ( self->mIndexBatchSize );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_getShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return 0;
}

//----------------------------------------------------------------//
// TODO
int MOAIMaterialBatch::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return 0;
}

//----------------------------------------------------------------//
// TODO
int MOAIMaterialBatch::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return 0;
}

//----------------------------------------------------------------//
// TODO
int MOAIMaterialBatch::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->mIndexBatchSize = state.GetValue < u32 >( 2, 1 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )

	u32 shaderIdx = 0;
	u32 param = 2;
	
	if ( state.IsType ( param, LUA_TNUMBER )) {
		shaderIdx = state.GetValue < u32 >( param++, 1 ) - 1;
	}

	MOAIShader* shader = 0;

	if ( state.IsType ( param, LUA_TNUMBER )) {
		shader = MOAIShaderMgr::Get ().GetShader ( state.GetValue < u32 >( param, MOAIShaderMgr::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShader >( 2, true );
	}

	self->SetShader ( shaderIdx, shader );
	state.Push ( shader );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )

	u32 textureIdx = 0;
	u32 param = 2;
	
	if ( state.IsType ( param, LUA_TNUMBER )) {
		textureIdx = state.GetValue < u32 >( param++, 1 ) - 1;
	}

	MOAIGfxState* texture = MOAITexture::AffirmTexture ( state, param );
	
	self->SetTexture ( textureIdx, texture );
	state.Push ( texture );
	return 1;
}

//================================================================//
// MOAIMaterialBatch
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialBatch::Clear () {

	for ( u32 i = 0; i < this->mMaterials.Size (); ++i ) {
		MOAIMaterial& material = this->mMaterials [ i ];
	
		this->LuaRelease ( material.mShader );
		this->LuaRelease ( material.mTexture );
	}
	this->mMaterials.Clear ();
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::LoadGfxState ( u32 idx, u32 defaultShader ) {

	if (this->mMaterials.Size ())  {
		idx = this->mIndexBatchSize > 0 ? ((( u32 )( idx / this->mIndexBatchSize )) % this->mMaterials.Size ()) : 0;
	}

	this->RawLoadGfxState ( idx, defaultShader );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::LoadGfxState ( u32 materialID, u32 deckIndex, u32 defaultShader ) {

	if ( materialID == UNKNOWN ) {
		this->LoadGfxState ( deckIndex, defaultShader );
	}
	else {
		this->RawLoadGfxState ( materialID, defaultShader );
	}
}

//----------------------------------------------------------------//
MOAIMaterialBatch::MOAIMaterialBatch () :
	mIndexBatchSize ( 1 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialBatch::~MOAIMaterialBatch () {
}

//----------------------------------------------------------------//
MOAIShader* MOAIMaterialBatch::RawGetShader ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		return this->mMaterials [ idx ].mShader;
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIGfxState* MOAIMaterialBatch::RawGetTexture ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		return this->mMaterials [ idx ].mTexture;
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getShader",				_getShader },
		{ "getTexture",				_getTexture },
		{ "reserve",				_reserve },
		{ "reserveTextures",		_reserve }, // back compat
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ "setShader",				_setShader },
		{ "setTexture",				_setTexture },
		{ "setTextureBatchSize",	_setIndexBatchSize }, // back compat
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::RawLoadGfxState ( u32 idx, u32 defaultShader ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	MOAIShader* shader = 0;
	MOAIGfxState* texture = 0;
	
	if ( idx < this->mMaterials.Size ()) {
		shader = this->mMaterials [ idx ].mShader;
		texture = this->mMaterials [ idx ].mTexture;
	}
	
	shader = shader ? shader : MOAIShaderMgr::Get ().GetShader ( defaultShader );
	
	gfxDevice.SetShader ( shader );
	gfxDevice.SetGfxState ( texture );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::Reserve ( u32 n ) {

	this->Clear ();
	this->mMaterials.Init ( n );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetShader ( u32 idx, u32 shaderID ) {

	this->SetShader ( idx, MOAIShaderMgr::Get ().GetShader ( shaderID ));
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetShader ( u32 idx, MOAIShader* shader ) {

	this->mMaterials.Grow ( idx, 1 );
	MOAIMaterial& material ( this->mMaterials [ idx ]);
	
	if ( material.mShader != shader ) {
	
		this->LuaRetain ( shader );
		this->LuaRelease ( material.mShader );
		material.mShader  = shader;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetTexture ( u32 idx, MOAIGfxState* texture ) {

	this->mMaterials.Grow ( idx, 1 );
	MOAIMaterial& material ( this->mMaterials [ idx ]);
	
	if ( material.mTexture != texture ) {
	
		this->LuaRetain ( texture );
		this->LuaRelease ( material.mTexture );
		material.mTexture  = texture;
	}
}

//----------------------------------------------------------------//
size_t MOAIMaterialBatch::Size () {

	return this->mMaterials.Size ();
}

