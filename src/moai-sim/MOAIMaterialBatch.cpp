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
void MOAIMaterial::LoadGfxState ( MOAIMaterial* fallback, u32 defaultShader ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	MOAIShader* shader = this->mShader ? this->mShader : (( fallback && fallback->mShader ) ? fallback->mShader : MOAIShaderMgr::Get ().GetShader ( defaultShader ));
	MOAIGfxState* texture = this->mTexture ? this->mTexture : (( fallback && fallback->mTexture ) ? fallback->mTexture : 0 );
	
	gfxDevice.SetShader ( shader );
	
	if ( texture ) {
		gfxDevice.SetGfxState ( texture );
	}
	else {
		gfxDevice.SetTexture ();
	}
}

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
	state.Push ( self->RawGetShader ( state.GetValue < u32 >( 2, 1 ) - 1 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO
int MOAIMaterialBatch::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->RawGetTexture ( state.GetValue < u32 >( 2, 1 ) - 1 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO
int MOAIMaterialBatch::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->Reserve ( state.GetValue ( 2, 0 ));
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
	state.Push ( self->SetShader ( state, 2 ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->SetTexture ( state, 2 ));
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
MOAIMaterial* MOAIMaterialBatch::GetMaterial ( u32 idx ) {

	size_t totalMaterials = this->mMaterials.Size ();

	if ( totalMaterials )  {
		idx = this->mIndexBatchSize > 0 ? ((( u32 )( idx / this->mIndexBatchSize )) % totalMaterials ) : 0;
	}
	return this->RawGetMaterial ( idx );
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatch::GetMaterial ( u32 materialID, u32 deckIndex ) {

	if ( materialID == UNKNOWN ) {
		return this->GetMaterial ( deckIndex );
	}
	return this->RawGetMaterial ( materialID );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::LoadGfxState ( MOAIMaterialBatch* fallback, u32 idx, u32 defaultShader ) {

	MOAIMaterial* primary = this->GetMaterial ( idx );
	MOAIMaterial* secondary = ( fallback && ( this != fallback )) ? fallback->GetMaterial ( idx ) : 0;

	primary->LoadGfxState ( secondary, defaultShader );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::LoadGfxState ( MOAIMaterialBatch* fallback, u32 materialID, u32 deckIndex, u32 defaultShader ) {

	MOAIMaterial* primary = this->GetMaterial ( materialID, deckIndex );
	MOAIMaterial* secondary = ( fallback && ( this != fallback )) ? fallback->GetMaterial ( materialID, deckIndex ) : 0;

	primary->LoadGfxState ( secondary, defaultShader );
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
MOAIMaterial* MOAIMaterialBatch::RawGetMaterial ( u32 idx ) {

	return ( idx < this->mMaterials.Size ()) ? &this->mMaterials [ idx ] : 0;
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
		{ "getIndexBatchSize",		_getIndexBatchSize },
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
	
	if ( texture ) {
		gfxDevice.SetGfxState ( texture );
	}
	else {
		gfxDevice.SetTexture ();
	}
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
MOAIShader* MOAIMaterialBatch::SetShader ( MOAILuaState& state, u32 idx ) {
	
	u32 shaderIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shaderIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIShader* shader = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shader = MOAIShaderMgr::Get ().GetShader ( state.GetValue < u32 >( idx, MOAIShaderMgr::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShader >( 2, true );
	}
	
	this->SetShader ( shaderIdx, shader );
	return shader;
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
MOAIGfxState* MOAIMaterialBatch::SetTexture ( MOAILuaState& state, u32 idx ) {
	
	u32 txtureIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		txtureIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIGfxState* texture = MOAITexture::AffirmTexture ( state, idx );
	this->SetTexture ( txtureIdx, texture );
	return texture;
}

//----------------------------------------------------------------//
size_t MOAIMaterialBatch::Size () {

	return this->mMaterials.Size ();
}

