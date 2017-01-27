// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionWorld.h>
#include <moai-sim/MOAIOverlap.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// MOAIOverlap
//================================================================//

//----------------------------------------------------------------//
void MOAIOverlap::Overlap ( const MOAIOverlapShape& shape0, const MOAIOverlapShape& shape1, MOAIOverlapHandler& handler ) {
	
	if ( Overlap ( shape0, shape1, handler.mTransform0, handler.mTransform1, handler.mCalculateBounds, handler.mBounds )) {
	
		handler.HandleOverlap ();
	}
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const MOAIOverlapShape& shape0, const MOAIOverlapShape& shape1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {

	bounds = ZLBounds::EMPTY;

	switch ( COLLISION_COMBO_CODE ( shape0.mType, shape1.mType )) {
	
		case BOX__BOX:				return Overlap ( *( ZLBox* )shape0.mPtr, *( ZLBox* )shape1.mPtr,				t0, t1, calculateBounds, bounds );
		case BOX__FRUSTUM:			return Overlap ( *( ZLBox* )shape0.mPtr, *( ZLFrustum* )shape1.mPtr,			t0, t1, calculateBounds, bounds );
		case BOX__PRISM:			return Overlap ( *( ZLBox* )shape0.mPtr, *( ZLPrism* )shape1.mPtr,				t0, t1, calculateBounds, bounds );
		case BOX__SPHERE:			return Overlap ( *( ZLBox* )shape0.mPtr, *( ZLSphere* )shape1.mPtr,				t0, t1, calculateBounds, bounds );

		case CIRCLE__CIRCLE:		return Overlap ( *( ZLCircle* )shape0.mPtr, *( ZLCircle* )shape1.mPtr,			t0, t1, calculateBounds, bounds );
		case CIRCLE__POLYGON:		return Overlap ( *( ZLCircle* )shape0.mPtr, *( ZLPolygon2D* )shape1.mPtr,		t0, t1, calculateBounds, bounds );
		case CIRCLE__QUAD:			return Overlap ( *( ZLCircle* )shape0.mPtr, *( ZLQuad* )shape1.mPtr,			t0, t1, calculateBounds, bounds );
		case CIRCLE__RECT:			return Overlap ( *( ZLCircle* )shape0.mPtr, *( ZLRect* )shape1.mPtr,			t0, t1, calculateBounds, bounds );

		case FRUSTUM__BOX:			return Overlap ( *( ZLBox* )shape1.mPtr, *( ZLFrustum* )shape0.mPtr,			t1, t0, calculateBounds, bounds );
		case FRUSTUM__FRUSTUM:		return Overlap ( *( ZLFrustum* )shape0.mPtr, *( ZLFrustum* )shape1.mPtr,		t0, t1, calculateBounds, bounds );
		case FRUSTUM__PRISM:		return Overlap ( *( ZLFrustum* )shape0.mPtr, *( ZLPrism* )shape1.mPtr,			t0, t1, calculateBounds, bounds );
		case FRUSTUM__SPHERE:		return Overlap ( *( ZLFrustum* )shape0.mPtr, *( ZLSphere* )shape1.mPtr,			t0, t1, calculateBounds, bounds );

		case POLYGON__CIRCLE:		return Overlap ( *( ZLCircle* )shape1.mPtr, *( ZLPolygon2D* )shape0.mPtr,		t1, t0, calculateBounds, bounds );
		case POLYGON__POLYGON:		return Overlap ( *( ZLPolygon2D* )shape0.mPtr, *( ZLPolygon2D* )shape1.mPtr,	t0, t1, calculateBounds, bounds );
		case POLYGON__QUAD:			return Overlap ( *( ZLPolygon2D* )shape0.mPtr, *( ZLQuad* )shape1.mPtr,			t0, t1, calculateBounds, bounds );
		case POLYGON__RECT:			return Overlap ( *( ZLPolygon2D* )shape0.mPtr, *( ZLRect* )shape1.mPtr,			t0, t1, calculateBounds, bounds );
		
		case PRISM__BOX:			return Overlap ( *( ZLBox* )shape1.mPtr, *( ZLPrism* )shape0.mPtr,				t1, t0, calculateBounds, bounds );
		case PRISM__FRUSTUM:		return Overlap ( *( ZLFrustum* )shape1.mPtr, *( ZLPrism* )shape0.mPtr,			t1, t0, calculateBounds, bounds );
		case PRISM__PRISM:			return Overlap ( *( ZLPrism* )shape0.mPtr, *( ZLPrism* )shape1.mPtr,			t0, t1, calculateBounds, bounds );
		case PRISM__SPHERE:			return Overlap ( *( ZLPrism* )shape0.mPtr, *( ZLSphere* )shape1.mPtr,			t0, t1, calculateBounds, bounds );
		
		case QUAD__CIRCLE:			return Overlap ( *( ZLCircle* )shape1.mPtr, *( ZLQuad* )shape0.mPtr,			t1, t0, calculateBounds, bounds );
		case QUAD__POLYGON:			return Overlap ( *( ZLPolygon2D* )shape1.mPtr, *( ZLQuad* )shape0.mPtr,			t1, t0, calculateBounds, bounds );
		case QUAD__QUAD:			return Overlap ( *( ZLQuad* )shape0.mPtr, *( ZLQuad* )shape1.mPtr,				t0, t1, calculateBounds, bounds );
		case QUAD__RECT:			return Overlap ( *( ZLQuad* )shape0.mPtr, *( ZLRect* )shape1.mPtr,				t0, t1, calculateBounds, bounds );

		case RECT__CIRCLE:			return Overlap ( *( ZLCircle* )shape1.mPtr, *( ZLRect* )shape0.mPtr,			t1, t0, calculateBounds, bounds );
		case RECT__POLYGON:			return Overlap ( *( ZLPolygon2D* )shape1.mPtr, *( ZLRect* )shape0.mPtr,			t1, t0, calculateBounds, bounds );
		case RECT__QUAD:			return Overlap ( *( ZLQuad* )shape1.mPtr, *( ZLRect* )shape0.mPtr,				t1, t0, calculateBounds, bounds );
		case RECT__RECT:			return Overlap ( *( ZLRect* )shape0.mPtr, *( ZLRect* )shape1.mPtr,				t0, t1, calculateBounds, bounds );

		case SPHERE__BOX:			return Overlap ( *( ZLBox* )shape1.mPtr, *( ZLSphere* )shape0.mPtr,				t1, t0, calculateBounds, bounds );
		case SPHERE__FRUSTUM:		return Overlap ( *( ZLFrustum* )shape1.mPtr, *( ZLSphere* )shape0.mPtr,			t1, t0, calculateBounds, bounds );
		case SPHERE__PRISM:			return Overlap ( *( ZLPrism* )shape1.mPtr, *( ZLSphere* )shape0.mPtr,			t1, t0, calculateBounds, bounds );
		case SPHERE__SPHERE:		return Overlap ( *( ZLSphere* )shape0.mPtr, *( ZLSphere* )shape1.mPtr,			t0, t1, calculateBounds, bounds );
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLBox& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return p0.Overlap ( p1 );
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLFrustum& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLPrism& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLSphere& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLCircle& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLPolygon2D& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLQuad& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLRect& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLFrustum& p0, const ZLFrustum& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLFrustum& p0, const ZLPrism& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLFrustum& p0, const ZLSphere& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPolygon2D& p0, const ZLPolygon2D& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPolygon2D& p0, const ZLQuad& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPolygon2D& p0, const ZLRect& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPrism& p0, const ZLPrism& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPrism& p0, const ZLSphere& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLQuad& p0, const ZLQuad& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {

	ZLQuad tq0 = p0;
	tq0.Transform ( t0.GetLocalToWorldMtx ());

	ZLQuad tq1 = p1;
	tq1.Transform ( t1.GetLocalToWorldMtx ());

	if ( calculateBounds ) {
		ZLRect rect;
		bool result = tq0.Intersect ( tq1, rect );
		if ( result ) {
			bounds.Init ( rect );
		}
		return result;
	}
	return tq0.Overlap ( tq1 );
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLQuad& p0, const ZLRect& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLRect& p0, const ZLRect& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( calculateBounds );
	UNUSED ( bounds );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLSphere& p0, const ZLSphere& p1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );
	UNUSED ( bounds );

	return false;
}
