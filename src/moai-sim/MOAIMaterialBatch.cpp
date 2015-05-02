// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMaterialBatch.h>
#include <moai-sim/MOAIProp.h>
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
	mTexture ( 0 ),
	mHitMask ( 0 ),
	mHitColorScalar ( 0xffffffff ),
	mHitColorThreshold ( 0x00000000 ) {
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {
}

//----------------------------------------------------------------//
bool MOAIMaterial::TestHit ( float x, float y ) {

	static const ZLColorVec defaultHitColor ( 0.0f, 0.0f, 0.0f, 1.0f );

	if ( this->mHitMask ) {
	
		float width = ( float )this->mHitMask->GetWidth ();
		float height = ( float )this->mHitMask->GetHeight ();
	
		ZLColorVec scalar ( this->mHitColorScalar );
		ZLColorVec threshold ( this->mHitColorThreshold );
	
		ZLColorVec maskColor ( this->mHitMask->GetColor (( u32 )( width * x ), ( u32 )( height * y ))); // TODO: wrap, clamp
		
		maskColor.Modulate ( scalar );
		
		return (
			( threshold.mR <= maskColor.mR ) &&
			( threshold.mG <= maskColor.mG ) &&
			( threshold.mB <= maskColor.mB ) &&
			( threshold.mA <= maskColor.mA )
		);
	}
	return true;
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
int MOAIMaterialBatch::_reserveMaterials ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->Reserve ( state.GetValue ( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setHitMask
	@text	Set or load an image to act as the hit mask for this deck.
	
	@in		MOAIDeck self
	@in		variant mask		A MOAIImage or a path to an image file
	@out	MOAIImage mask
*/
int MOAIMaterialBatch::_setHitMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )
	state.Push ( self->SetHitMask ( state, 2 ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setHitMaskScalar ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )
	self->SetHitMaskScalar ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setHitMaskThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )
	self->SetHitMaskThreshold ( state, 2 );
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
MOAIMaterial& MOAIMaterialBatch::AffirmMaterial ( u32 idx ) {

	this->mMaterials.Grow ( idx, 1 );
	return this->mMaterials [ idx ];
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::Clear () {

	for ( u32 i = 0; i < this->mMaterials.Size (); ++i ) {
		MOAIMaterial& material = this->mMaterials [ i ];
	
		this->LuaRelease ( material.mShader );
		this->LuaRelease ( material.mTexture );
		this->LuaRelease ( material.mHitMask );
	}
	this->mMaterials.Clear ();
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatch::GetMaterial ( u32 idx ) {

	return this->RawGetMaterial ( this->GetRawIndex ( idx ));
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

	this->LoadGfxState ( fallback, UNKNOWN, idx, defaultShader );
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
		{ "reserveMaterials",		_reserveMaterials },
		{ "setHitMask",				_setHitMask },
		{ "setHitMaskScalar",		_setHitMaskScalar },
		{ "setHitMaskThreshold",	_setHitMaskThreshold },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ "setShader",				_setShader },
		{ "setTexture",				_setTexture },
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
void MOAIMaterialBatch::SetHitMask ( u32 idx, MOAIImage* mask ) {

	MOAIMaterial& material = this->AffirmMaterial ( idx );
	
	if ( material.mHitMask != mask ) {
	
		this->LuaRetain ( mask );
		this->LuaRelease ( material.mHitMask );
		material.mHitMask  = mask;
	}
}

//----------------------------------------------------------------//
MOAIImage* MOAIMaterialBatch::SetHitMask ( MOAILuaState& state, u32 idx ) {

	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIImage* mask = MOAIImage::AffirmImage ( state, idx );
	this->SetHitMask ( materialIdx, mask );
	return mask;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMaskScalar ( u32 idx, u32 scalar ) {

	this->AffirmMaterial ( idx ).mHitColorScalar = scalar;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMaskScalar ( MOAILuaState& state, u32 idx ) {

	u32 materialIdx = 0;
	if ( state.CheckParams ( idx, "NNNNN", false )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	ZLColorVec scalar = state.GetValue < ZLColorVec >( idx, ZLColorVec ( 1.0f, 1.0f, 1.0f, 1.0f ));
	this->SetHitMaskScalar ( materialIdx, scalar.PackRGBA ());
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMaskThreshold ( u32 idx, u32 threshold ) {

	this->AffirmMaterial ( idx ).mHitColorThreshold = threshold;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMaskThreshold ( MOAILuaState& state, u32 idx ) {

	u32 materialIdx = 0;
	if ( state.CheckParams ( idx, "NNNNN", false )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	ZLColorVec threshold = state.GetValue < ZLColorVec >( idx, ZLColorVec ( 0.0f, 0.0f, 0.0f, 0.0f ));
	this->SetHitMaskThreshold ( materialIdx, threshold.PackRGBA ());
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

	MOAIMaterial& material = this->AffirmMaterial ( idx );
	if ( material.mShader != shader ) {
	
		this->LuaRetain ( shader );
		this->LuaRelease ( material.mShader );
		material.mShader  = shader;
	}
}

//----------------------------------------------------------------//
MOAIShader* MOAIMaterialBatch::SetShader ( MOAILuaState& state, u32 idx ) {
	
	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIShader* shader = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shader = MOAIShaderMgr::Get ().GetShader ( state.GetValue < u32 >( idx, MOAIShaderMgr::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShader >( 2, true );
	}
	
	this->SetShader ( materialIdx, shader );
	return shader;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetTexture ( u32 idx, MOAIGfxState* texture ) {

	MOAIMaterial& material = this->AffirmMaterial ( idx );
	if ( material.mTexture != texture ) {
	
		this->LuaRetain ( texture );
		this->LuaRelease ( material.mTexture );
		material.mTexture  = texture;
	}
}

//----------------------------------------------------------------//
MOAIGfxState* MOAIMaterialBatch::SetTexture ( MOAILuaState& state, u32 idx ) {
	
	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	MOAIGfxState* texture = MOAITexture::AffirmTexture ( state, idx );
	this->SetTexture ( materialIdx, texture );
	return texture;
}

//----------------------------------------------------------------//
size_t MOAIMaterialBatch::Size () {

	return this->mMaterials.Size ();
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::TestHit ( MOAIMaterialBatch* fallback, u32 idx, float x, float y ) {

	return this->TestHit ( fallback, UNKNOWN, idx, x, y );
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::TestHit ( MOAIMaterialBatch* fallback, u32 materialID, u32 deckIndex, float x, float y ) {

	MOAIMaterial* primary = this->GetMaterial ( materialID, deckIndex );
	if ( primary && primary->mHitMask ) {
		return primary->TestHit ( x, y );
	}
	
	MOAIMaterial* secondary = ( fallback && ( this != fallback )) ? fallback->GetMaterial ( materialID, deckIndex ) : 0;
	if ( secondary && secondary->mHitMask ) {
		return secondary->TestHit ( x, y );
	}
	
	return 0;
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::TestHit ( MOAIMaterialBatch* fallback, u32 idx, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	return this->TestHit ( fallback, UNKNOWN, idx, granularity, modelQuad, uvQuad, x, y );
}

//----------------------------------------------------------------//
bool MOAIMaterialBatch::TestHit ( MOAIMaterialBatch* fallback, u32 materialID, u32 deckIndex, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	if ( granularity == MOAIProp::HIT_TEST_COARSE ) return true;

	ZLVec2D uv;

	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 0, x, y, uv)) {
		return granularity == MOAIProp::HIT_TEST_FINE ? this->TestHit ( fallback, materialID, deckIndex, uv.mX, uv.mY ) : true;
	}
	
	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 1, x, y, uv)) {
		return granularity == MOAIProp::HIT_TEST_FINE ? this->TestHit ( fallback, materialID, deckIndex, uv.mX, uv.mY ) : true;
	}

	return false;
}
