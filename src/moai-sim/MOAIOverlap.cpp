// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIOverlap.h>

using namespace MOAICollisionConsts;

//================================================================//
// MOAIOverlap Overlap
//================================================================//

//----------------------------------------------------------------//
MOAIOverlap::MOAIOverlap () :
	mCalculateBounds ( false ) {
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLBox& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return p0.Overlap ( p1 );
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLFrustum& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLPrism& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLBox& p0, const ZLSphere& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLCircle& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLPolygon2D& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLQuad& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLCircle& p0, const ZLRect& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLFrustum& p0, const ZLFrustum& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLFrustum& p0, const ZLPrism& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLFrustum& p0, const ZLSphere& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPolygon2D& p0, const ZLPolygon2D& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPolygon2D& p0, const ZLQuad& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPolygon2D& p0, const ZLRect& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPrism& p0, const ZLPrism& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLPrism& p0, const ZLSphere& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLQuad& p0, const ZLQuad& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {

	ZLQuad tq0 = p0;
	tq0.Transform ( t0 );

	ZLQuad tq1 = p1;
	tq1.Transform ( t1 );

	if ( this->mCalculateBounds ) {
	
		ZLRect rect;
		if ( tq0.Intersect ( tq1, rect )) {
			this->MOAIOverlap_OnOverlap ( ZLBounds ( rect ));
			return true;
		}
	}
	else if ( tq0.Overlap ( tq1 )) {
	
		this->MOAIOverlap_OnOverlap ( ZLBounds::EMPTY );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLQuad& p0, const ZLRect& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLRect& p0, const ZLRect& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//----------------------------------------------------------------//
bool MOAIOverlap::Overlap ( const ZLSphere& p0, const ZLSphere& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	UNUSED ( p0 );
	UNUSED ( p1 );
	UNUSED ( t0 );
	UNUSED ( t1 );

	return false;
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIOverlap::MOAICollisionPrimVisitor_Process ( const MOAICollisionPrim& shape0, const MOAICollisionPrim& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {

	switch ( COLLISION_COMBO_CODE ( shape0.mType, shape1.mType )) {
	
		case BOX__BOX:				Overlap ( *( ZLBox* )shape0.mPtr, *( ZLBox* )shape1.mPtr,				t0, t1 );
		case BOX__FRUSTUM:			Overlap ( *( ZLBox* )shape0.mPtr, *( ZLFrustum* )shape1.mPtr,			t0, t1 );
		case BOX__PRISM:			Overlap ( *( ZLBox* )shape0.mPtr, *( ZLPrism* )shape1.mPtr,				t0, t1 );
		case BOX__SPHERE:			Overlap ( *( ZLBox* )shape0.mPtr, *( ZLSphere* )shape1.mPtr,			t0, t1 );

		case CIRCLE__CIRCLE:		Overlap ( *( ZLCircle* )shape0.mPtr, *( ZLCircle* )shape1.mPtr,			t0, t1 );
		case CIRCLE__POLYGON:		Overlap ( *( ZLCircle* )shape0.mPtr, *( ZLPolygon2D* )shape1.mPtr,		t0, t1 );
		case CIRCLE__QUAD:			Overlap ( *( ZLCircle* )shape0.mPtr, *( ZLQuad* )shape1.mPtr,			t0, t1 );
		case CIRCLE__RECT:			Overlap ( *( ZLCircle* )shape0.mPtr, *( ZLRect* )shape1.mPtr,			t0, t1 );

		case FRUSTUM__BOX:			Overlap ( *( ZLBox* )shape1.mPtr, *( ZLFrustum* )shape0.mPtr,			t1, t0 );
		case FRUSTUM__FRUSTUM:		Overlap ( *( ZLFrustum* )shape0.mPtr, *( ZLFrustum* )shape1.mPtr,		t0, t1 );
		case FRUSTUM__PRISM:		Overlap ( *( ZLFrustum* )shape0.mPtr, *( ZLPrism* )shape1.mPtr,			t0, t1 );
		case FRUSTUM__SPHERE:		Overlap ( *( ZLFrustum* )shape0.mPtr, *( ZLSphere* )shape1.mPtr,		t0, t1 );

		case POLYGON__CIRCLE:		Overlap ( *( ZLCircle* )shape1.mPtr, *( ZLPolygon2D* )shape0.mPtr,		t1, t0 );
		case POLYGON__POLYGON:		Overlap ( *( ZLPolygon2D* )shape0.mPtr, *( ZLPolygon2D* )shape1.mPtr,	t0, t1 );
		case POLYGON__QUAD:			Overlap ( *( ZLPolygon2D* )shape0.mPtr, *( ZLQuad* )shape1.mPtr,		t0, t1 );
		case POLYGON__RECT:			Overlap ( *( ZLPolygon2D* )shape0.mPtr, *( ZLRect* )shape1.mPtr,		t0, t1 );
		
		case PRISM__BOX:			Overlap ( *( ZLBox* )shape1.mPtr, *( ZLPrism* )shape0.mPtr,				t1, t0 );
		case PRISM__FRUSTUM:		Overlap ( *( ZLFrustum* )shape1.mPtr, *( ZLPrism* )shape0.mPtr,			t1, t0 );
		case PRISM__PRISM:			Overlap ( *( ZLPrism* )shape0.mPtr, *( ZLPrism* )shape1.mPtr,			t0, t1 );
		case PRISM__SPHERE:			Overlap ( *( ZLPrism* )shape0.mPtr, *( ZLSphere* )shape1.mPtr,			t0, t1 );
		
		case QUAD__CIRCLE:			Overlap ( *( ZLCircle* )shape1.mPtr, *( ZLQuad* )shape0.mPtr,			t1, t0 );
		case QUAD__POLYGON:			Overlap ( *( ZLPolygon2D* )shape1.mPtr, *( ZLQuad* )shape0.mPtr,		t1, t0 );
		case QUAD__QUAD:			Overlap ( *( ZLQuad* )shape0.mPtr, *( ZLQuad* )shape1.mPtr,				t0, t1 );
		case QUAD__RECT:			Overlap ( *( ZLQuad* )shape0.mPtr, *( ZLRect* )shape1.mPtr,				t0, t1 );

		case RECT__CIRCLE:			Overlap ( *( ZLCircle* )shape1.mPtr, *( ZLRect* )shape0.mPtr,			t1, t0 );
		case RECT__POLYGON:			Overlap ( *( ZLPolygon2D* )shape1.mPtr, *( ZLRect* )shape0.mPtr,		t1, t0 );
		case RECT__QUAD:			Overlap ( *( ZLQuad* )shape1.mPtr, *( ZLRect* )shape0.mPtr,				t1, t0 );
		case RECT__RECT:			Overlap ( *( ZLRect* )shape0.mPtr, *( ZLRect* )shape1.mPtr,				t0, t1 );

		case SPHERE__BOX:			Overlap ( *( ZLBox* )shape1.mPtr, *( ZLSphere* )shape0.mPtr,			t1, t0 );
		case SPHERE__FRUSTUM:		Overlap ( *( ZLFrustum* )shape1.mPtr, *( ZLSphere* )shape0.mPtr,		t1, t0 );
		case SPHERE__PRISM:			Overlap ( *( ZLPrism* )shape1.mPtr, *( ZLSphere* )shape0.mPtr,			t1, t0 );
		case SPHERE__SPHERE:		Overlap ( *( ZLSphere* )shape0.mPtr, *( ZLSphere* )shape1.mPtr,			t0, t1 );
	}
}
