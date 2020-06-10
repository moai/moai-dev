// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIPartitionHull.h>

SUPPRESS_EMPTY_FILE_WARNING

#if 0

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
// MOAIGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::SetHitMask ( u32 idx, MOAIImage* mask ) {

	MOAIMaterial& material = this->AffirmMaterial ( idx );
	
	if ( material.mHitMask != mask ) {
	
		this->LuaRetain ( mask );
		this->LuaRelease ( material.mHitMask );
		material.mHitMask  = mask;
	}
}

//----------------------------------------------------------------//
MOAIImage* MOAIGfxScriptBatch::SetHitMask ( MOAILuaState& state, u32 idx ) {

	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIImage* mask = MOAIImage::AffirmImage ( state, idx );
	this->SetHitMask ( materialIdx, mask );
	return mask;
}

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::SetHitMaskScalar ( u32 idx, u32 scalar ) {

	this->AffirmMaterial ( idx ).mHitColorScalar = scalar;
}

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::SetHitMaskScalar ( MOAILuaState& state, u32 idx ) {

	u32 materialIdx = 0;
	if ( state.CheckParams ( idx, "NNNNN", false )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	ZLColorVec scalar = state.GetValue < ZLColorVec >( idx, ZLColorVec::WHITE );
	this->SetHitMaskScalar ( materialIdx, scalar.PackRGBA ());
}

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::SetHitMaskThreshold ( u32 idx, u32 threshold ) {

	this->AffirmMaterial ( idx ).mHitColorThreshold = threshold;
}

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::SetHitMaskThreshold ( MOAILuaState& state, u32 idx ) {

	u32 materialIdx = 0;
	if ( state.CheckParams ( idx, "NNNNN", false )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	ZLColorVec threshold = state.GetValue < ZLColorVec >( idx, ZLColorVec ( 0.0f, 0.0f, 0.0f, 0.0f ));
	this->SetHitMaskThreshold ( materialIdx, threshold.PackRGBA ());
}

//----------------------------------------------------------------//
bool MOAIGfxScriptBatch::TestHit ( MOAIGfxScriptBatch* override, u32 idx, float x, float y ) {

	return this->TestHit ( override, UNKNOWN, idx, x, y );
}

//----------------------------------------------------------------//
bool MOAIGfxScriptBatch::TestHit ( MOAIGfxScriptBatch* override, u32 materialID, u32 deckIndex, float x, float y ) {

	MOAIMaterial* primary = ( override && ( this != override )) ? override->GetMaterial ( materialID, deckIndex ) : 0;
	if ( primary && primary->mHitMask ) {
		return primary->TestHit ( x, y );
	}
	
	MOAIMaterial* secondary = this->GetMaterial ( materialID, deckIndex );
	if ( secondary && secondary->mHitMask ) {
		return secondary->TestHit ( x, y );
	}
	
	return 0;
}

//----------------------------------------------------------------//
bool MOAIGfxScriptBatch::TestHit ( MOAIGfxScriptBatch* override, u32 idx, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	return this->TestHit ( override, UNKNOWN, idx, granularity, modelQuad, uvQuad, x, y );
}

//----------------------------------------------------------------//
bool MOAIGfxScriptBatch::TestHit ( MOAIGfxScriptBatch* override, u32 materialID, u32 deckIndex, u32 granularity, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	if ( granularity == MOAIPartitionHull::HIT_TEST_COARSE ) return true;

	ZLVec2D uv;

	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 0, x, y, uv)) {
		return granularity == MOAIPartitionHull::HIT_TEST_FINE ? this->TestHit ( override, materialID, deckIndex, uv.mX, uv.mY ) : true;
	}
	
	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 1, x, y, uv)) {
		return granularity == MOAIPartitionHull::HIT_TEST_FINE ? this->TestHit ( override, materialID, deckIndex, uv.mX, uv.mY ) : true;
	}

	return false;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

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

#endif
