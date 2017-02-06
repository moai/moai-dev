// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIOverlapResolver.h>

using namespace MOAICollisionConsts;

//================================================================//
// MOAIOverlapResolver
//================================================================//

//----------------------------------------------------------------//
void MOAIOverlapResolver::Accumulate ( float x, float y, float z ) {

	this->mAccumulator.mX += x;
	this->mAccumulator.mY += y;
	this->mAccumulator.mZ += z;
	
	this->mCount += 1.0f;
}

//----------------------------------------------------------------//
ZLVec3D MOAIOverlapResolver::GetResult () {

	if ( this->mCount > 0.0f ) {
		ZLVec3D average = this->mAccumulator;
		average.Scale ( 1.0f / this->mCount );
		return average;
	}
	return ZLVec3D::ORIGIN;
}

//----------------------------------------------------------------//
MOAIOverlapResolver::MOAIOverlapResolver () :
	mAccumulator ( ZLVec3D::ORIGIN ),
	mCount ( 0.0f ) {
}

//----------------------------------------------------------------//
void MOAIOverlapResolver::Reset () {

	this->mAccumulator = ZLVec3D::ORIGIN;
	this->mCount = 0.0f;
}

//----------------------------------------------------------------//
bool MOAIOverlapResolver::Separate ( const ZLQuad& p0, const ZLQuad& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {

	ZLQuad tq0 = p0;
	tq0.Transform ( t0 );

	ZLQuad tq1 = p1;
	tq1.Transform ( t1 );

	return this->Separate ( tq0.mV, 4, tq1.mV, 4 );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIOverlapResolver::MOAIOverlapShapeVisitor_Process ( const MOAIOverlapShape& shape0, const MOAIOverlapShape& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {

	switch ( COLLISION_COMBO_CODE ( shape0.mType, shape1.mType )) {
	
		case QUAD__QUAD:			Separate ( *( ZLQuad* )shape0.mPtr, *( ZLQuad* )shape1.mPtr, t0, t1 );
	}
}

