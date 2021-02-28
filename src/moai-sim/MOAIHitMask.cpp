// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIHitMask.h>

//================================================================//
// MOAIHitMask
//================================================================//

//----------------------------------------------------------------//
MOAIHitMask::MOAIHitMask ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIAbstractHasHitMask ( context ),
	MOAIAbstractHitMask ( context ) {

	RTTI_BEGIN ( MOAIHitMask )
		RTTI_EXTEND ( MOAIAbstractHasHitMask )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHitMask::~MOAIHitMask () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractHitMask& MOAIHitMask::MOAIAbstractHasHitMask_AffirmHitMask () {

	return *this;
}

//----------------------------------------------------------------//
void MOAIHitMask::MOAIAbstractHitMask_SetImage ( MOAIImage* image ) {

	this->mImage = image;
}

//----------------------------------------------------------------//
void MOAIHitMask::MOAIAbstractHitMask_SetScalar ( u32 scalar ) {

	this->mHitColorScalar = scalar;
}

//----------------------------------------------------------------//
void MOAIHitMask::MOAIAbstractHitMask_SetThreshold ( u32 threshold ) {

	this->mHitColorThreshold = threshold;
}

//----------------------------------------------------------------//
bool MOAIHitMask::MOAIAbstractHitMask_TestHit ( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	if ( !this->mImage ) return false;

	ZLVec2D uv;

	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 0, x, y, uv)) {
		return this->TestHit ( uv.mX, uv.mY );
	}
	
	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 1, x, y, uv)) {
		return this->TestHit ( uv.mX, uv.mY );
	}

	return false;
}

//----------------------------------------------------------------//
bool MOAIHitMask::MOAIAbstractHitMask_TestHit ( float x, float y ) {

	if ( !this->mImage ) return false;

	static const ZLColorVec defaultHitColor ( ZLColorVec::BLACK );
	
	float width = ( float )this->mImage->GetWidth ();
	float height = ( float )this->mImage->GetHeight ();

	ZLColorVec scalar ( this->mHitColorScalar );
	ZLColorVec threshold ( this->mHitColorThreshold );

	ZLColorVec maskColor ( this->mImage->GetColor (( u32 )( width * x ), ( u32 )( height * y )));
	
	maskColor.Modulate ( scalar );
	
	return (
		( threshold.mR <= maskColor.mR ) &&
		( threshold.mG <= maskColor.mG ) &&
		( threshold.mB <= maskColor.mB ) &&
		( threshold.mA <= maskColor.mA )
	);
}
