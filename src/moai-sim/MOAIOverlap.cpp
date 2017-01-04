// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIOverlap.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// MOAIOverlap
//================================================================//

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const MOAIOverlapShape& selfShape, const MOAIOverlapShape& otherShape, const MOAITransformBase& selfTransform, const MOAITransformBase& otherTransform, ZLBox& bounds ) {

	switch ( COLLISION_COMBO_CODE ( selfShape.mType, otherShape.mType )) {
	
		case MOAIOverlap::BOX__BOX:				return MOAIOverlap::Overlap ( *( ZLBox* )selfShape.mPtr, *( ZLBox* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::BOX__FRUSTUM:			return MOAIOverlap::Overlap ( *( ZLBox* )selfShape.mPtr, *( ZLFrustum* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::BOX__PRISM:			return MOAIOverlap::Overlap ( *( ZLBox* )selfShape.mPtr, *( ZLPrism* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::BOX__SPHERE:			return MOAIOverlap::Overlap ( *( ZLBox* )selfShape.mPtr, *( ZLSphere* )otherShape.mPtr, selfTransform, otherTransform, bounds );

		case MOAIOverlap::CIRCLE__CIRCLE:		return MOAIOverlap::Overlap ( *( ZLCircle* )selfShape.mPtr, *( ZLCircle* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::CIRCLE__POLYGON:		return MOAIOverlap::Overlap ( *( ZLCircle* )selfShape.mPtr, *( ZLPolygon2D* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::CIRCLE__QUAD:			return MOAIOverlap::Overlap ( *( ZLCircle* )selfShape.mPtr, *( ZLQuad* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::CIRCLE__RECT:			return MOAIOverlap::Overlap ( *( ZLCircle* )selfShape.mPtr, *( ZLRect* )otherShape.mPtr, selfTransform, otherTransform, bounds );

		case MOAIOverlap::FRUSTUM__BOX:			return MOAIOverlap::Overlap ( *( ZLBox* )otherShape.mPtr, *( ZLFrustum* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::FRUSTUM__FRUSTUM:		return MOAIOverlap::Overlap ( *( ZLFrustum* )selfShape.mPtr, *( ZLFrustum* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::FRUSTUM__PRISM:		return MOAIOverlap::Overlap ( *( ZLFrustum* )selfShape.mPtr, *( ZLPrism* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::FRUSTUM__SPHERE:		return MOAIOverlap::Overlap ( *( ZLFrustum* )selfShape.mPtr, *( ZLSphere* )otherShape.mPtr, selfTransform, otherTransform, bounds );

		case MOAIOverlap::POLYGON__CIRCLE:		return MOAIOverlap::Overlap ( *( ZLCircle* )otherShape.mPtr, *( ZLPolygon2D* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::POLYGON__POLYGON:		return MOAIOverlap::Overlap ( *( ZLPolygon2D* )selfShape.mPtr, *( ZLPolygon2D* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::POLYGON__QUAD:		return MOAIOverlap::Overlap ( *( ZLPolygon2D* )selfShape.mPtr, *( ZLQuad* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::POLYGON__RECT:		return MOAIOverlap::Overlap ( *( ZLPolygon2D* )selfShape.mPtr, *( ZLRect* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		
		case MOAIOverlap::PRISM__BOX:			return MOAIOverlap::Overlap ( *( ZLBox* )otherShape.mPtr, *( ZLPrism* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::PRISM__FRUSTUM:		return MOAIOverlap::Overlap ( *( ZLFrustum* )otherShape.mPtr, *( ZLPrism* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::PRISM__PRISM:			return MOAIOverlap::Overlap ( *( ZLPrism* )selfShape.mPtr, *( ZLPrism* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::PRISM__SPHERE:		return MOAIOverlap::Overlap ( *( ZLPrism* )selfShape.mPtr, *( ZLSphere* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		
		case MOAIOverlap::QUAD__CIRCLE:			return MOAIOverlap::Overlap ( *( ZLCircle* )otherShape.mPtr, *( ZLQuad* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::QUAD__POLYGON:		return MOAIOverlap::Overlap ( *( ZLPolygon2D* )otherShape.mPtr, *( ZLQuad* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::QUAD__QUAD:			return MOAIOverlap::Overlap ( *( ZLQuad* )selfShape.mPtr, *( ZLQuad* )otherShape.mPtr, selfTransform, otherTransform, bounds );
		case MOAIOverlap::QUAD__RECT:			return MOAIOverlap::Overlap ( *( ZLQuad* )selfShape.mPtr, *( ZLRect* )otherShape.mPtr, selfTransform, otherTransform, bounds );

		case MOAIOverlap::RECT__CIRCLE:			return MOAIOverlap::Overlap ( *( ZLCircle* )otherShape.mPtr, *( ZLRect* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::RECT__POLYGON:		return MOAIOverlap::Overlap ( *( ZLPolygon2D* )otherShape.mPtr, *( ZLRect* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::RECT__QUAD:			return MOAIOverlap::Overlap ( *( ZLQuad* )otherShape.mPtr, *( ZLRect* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::RECT__RECT:			return MOAIOverlap::Overlap ( *( ZLRect* )selfShape.mPtr, *( ZLRect* )otherShape.mPtr, selfTransform, otherTransform, bounds );

		case MOAIOverlap::SPHERE__BOX:			return MOAIOverlap::Overlap ( *( ZLBox* )otherShape.mPtr, *( ZLSphere* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::SPHERE__FRUSTUM:		return MOAIOverlap::Overlap ( *( ZLFrustum* )otherShape.mPtr, *( ZLSphere* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::SPHERE__PRISM:		return MOAIOverlap::Overlap ( *( ZLPrism* )otherShape.mPtr, *( ZLSphere* )selfShape.mPtr, otherTransform, selfTransform, bounds );
		case MOAIOverlap::SPHERE__SPHERE:		return MOAIOverlap::Overlap ( *( ZLSphere* )selfShape.mPtr, *( ZLSphere* )otherShape.mPtr, selfTransform, otherTransform, bounds );
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLBox& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return p0.Overlap ( p1 );
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLFrustum& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLPrism& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLSphere& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLCircle& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLPolygon2D& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLQuad& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLRect& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLFrustum& p0, const ZLFrustum& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLFrustum& p0, const ZLPrism& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLFrustum& p0, const ZLSphere& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPolygon2D& p0, const ZLPolygon2D& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPolygon2D& p0, const ZLQuad& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPolygon2D& p0, const ZLRect& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPrism& p0, const ZLPrism& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPrism& p0, const ZLSphere& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLQuad& p0, const ZLQuad& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {

	ZLQuad tq0 = p0;
	tq0.Transform ( t0.GetLocalToWorldMtx ());

	ZLQuad tq1 = p1;
	tq1.Transform ( t1.GetLocalToWorldMtx ());

	ZLRect rect;
	bool result = tq0.Intersect ( tq1, rect );
	bounds.Init ( rect, ZLBox::PLANE_XY, 0.0f, 0.0f );
	return result;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLQuad& p0, const ZLRect& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLRect& p0, const ZLRect& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLSphere& p0, const ZLSphere& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, ZLBox& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}
