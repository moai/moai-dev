// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// MOAICollisionShape
//================================================================//

//----------------------------------------------------------------//
MOAICollisionShape::MOAICollisionShape () :
	mType ( NONE ) {
}

//----------------------------------------------------------------//
MOAICollisionShape::~MOAICollisionShape () {
}

//----------------------------------------------------------------//
bool MOAICollisionShape::Overlap ( const MOAITransformBase& selfTransform, const MOAITransformBase& otherTransform, const MOAICollisionShape& otherShape, ZLBox& bounds ) const {

	switch ( this->mType ) {
	
		case QUAD: {
			switch ( otherShape.mType ) {
				case QUAD: MOAICollisionShape::Overlap ( selfTransform, this->mQuad, otherTransform, otherShape.mQuad, bounds );
			}
			break;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set () {

	this->mType = NONE;
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( const ZLRect& aabb ) {

	this->mAABB = aabb;
	this->mType = AABB;
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( const ZLBox& box ) {

	this->mBox = box;
	this->mType = BOX;
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( const ZLQuad& quad ) {

	this->mQuad = quad;
	this->mType = QUAD;
}

//----------------------------------------------------------------//
bool MOAICollisionShape::Overlap ( const MOAITransformBase& t0, const ZLQuad& q0, const MOAITransformBase& t1, const ZLQuad& q1, ZLBox& bounds ) {

	ZLQuad tq0 = q0;
	tq0.Transform ( t0.GetLocalToWorldMtx ());

	ZLQuad tq1 = q1;
	tq1.Transform ( t1.GetLocalToWorldMtx ());

	ZLRect rect;
	bool result = tq0.Intersect ( tq1, rect );
	bounds.Init ( rect, ZLBox::PLANE_XY, 0.0f, 0.0f );
	return result;
}
