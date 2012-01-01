// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USFrustum.h>
#include <uslscore/USDistance.h>
#include <uslscore/USIntersect.h>
#include <uslscore/USMathConsts.h>
#include <uslscore/USPrism.h>
#include <uslscore/USRhombus.h>

//================================================================//
// USFrustum
//================================================================//

//----------------------------------------------------------------//
bool USFrustum::Cull ( USVec3D& vec ) {

	// Try to casually cull the frustum by the extents of the box
	// i.e. we can bail if the frustum AABB doesn't overlap the box
	if ( !this->mAABB.Contains ( vec )) return true;

	// No luck, so try to cull the box by the planes of the frustum
	if ( USDist::VecToPlane ( vec, mPlanes [ NEAR_PLANE ]) < 0.0f ) return true;
	if ( USDist::VecToPlane ( vec, mPlanes [ FAR_PLANE ]) < 0.0f ) return true;
	if ( USDist::VecToPlane ( vec, mPlanes [ LEFT_PLANE ]) < 0.0f ) return true;
	if ( USDist::VecToPlane ( vec, mPlanes [ RIGHT_PLANE ]) < 0.0f ) return true;
	if ( USDist::VecToPlane ( vec, mPlanes [ TOP_PLANE ]) < 0.0f ) return true;
	if ( USDist::VecToPlane ( vec, mPlanes [ BOTTOM_PLANE ]) < 0.0f ) return true;

	// At this point it looks like we can't cull, so return false
	return false;
}

//----------------------------------------------------------------//
bool USFrustum::Cull ( USBox& box ) {

	// Try to casually cull the frustum by the extents of the box
	// i.e. we can bail if the frustum AABB doesn't overlap the box
	if ( !box.Overlap ( mAABB )) return true;

	// No luck, so try to cull the box by the planes of the frustum
	if ( USSect::BoxToPlane ( box, mPlanes [ NEAR_PLANE ]) < 0 ) return true;
	if ( USSect::BoxToPlane ( box, mPlanes [ FAR_PLANE ]) < 0 ) return true;
	if ( USSect::BoxToPlane ( box, mPlanes [ LEFT_PLANE ]) < 0 ) return true;
	if ( USSect::BoxToPlane ( box, mPlanes [ RIGHT_PLANE ]) < 0 ) return true;
	if ( USSect::BoxToPlane ( box, mPlanes [ TOP_PLANE ]) < 0 ) return true;
	if ( USSect::BoxToPlane ( box, mPlanes [ BOTTOM_PLANE ]) < 0 ) return true;

	// At this point it looks like we can't cull, so return false
	return false;
}

//----------------------------------------------------------------//
bool USFrustum::Cull ( USPrism& prism ) {

	// No luck, so try to cull the box by the planes of the frustum
	if ( USSect::PrismToPlane ( prism, mPlanes [ LEFT_PLANE ]) < 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ RIGHT_PLANE ]) < 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ TOP_PLANE ]) < 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ BOTTOM_PLANE ]) < 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ NEAR_PLANE ]) < 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ FAR_PLANE ]) < 0 ) return true;

	// At this point it looks like we can't cull, so return false
	return false;
}

//----------------------------------------------------------------//
bool USFrustum::Cull ( USRhombus& rhombus ) {

	// No luck, so try to cull the box by the planes of the frustum
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ LEFT_PLANE ]) < 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ RIGHT_PLANE ]) < 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ TOP_PLANE ]) < 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ BOTTOM_PLANE ]) < 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ NEAR_PLANE ]) < 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ FAR_PLANE ]) < 0 ) return true;

	// At this point it looks like we can't cull, so return false
	return false;
}

//----------------------------------------------------------------//
void USFrustum::Init ( USVec3D& loc, USRhombus& rhombus, USPlane3D& near, USPlane3D& far ) {

	this->mLoc = loc;

	// Init near & far planes
	this->mPlanes [ NEAR_PLANE ] = near;
	this->mPlanes [ FAR_PLANE ] = far;

	// Get the plane containing the rhombus
	USPlane3D rhomPlane;
	rhombus.GetPlane ( rhomPlane );
	
	// Bless the rhombus
	USRhombus blessedRhombus = rhombus;
	if ( USDist::VecToPlane ( loc, rhomPlane ) < 0.0f ) {
	
		// We're behind the rhombus plane; the plane's normal is facing away from us
		// So flip the plane normal as well as the rhombus' Y axis
		rhomPlane.mNorm.Invert ();
		blessedRhombus.mLoc.Add ( blessedRhombus.mYAxis );
		blessedRhombus.mYAxis.Invert ();
	}

	// Now all our calculations will be correct!
	// And all your base are belong to us!  Hooray! 

	// Get vertices of rhombus
	USVec3D leftBottom = blessedRhombus.mLoc;
	
	USVec3D rightBottom = leftBottom;
	rightBottom.Add ( blessedRhombus.mXAxis );
	
	USVec3D rightTop = rightBottom;
	rightTop.Add ( blessedRhombus.mYAxis );
	
	USVec3D leftTop = rightTop;
	leftTop.Sub ( blessedRhombus.mXAxis );
	
	// Init the side planes
	this->mPlanes [ BOTTOM_PLANE ].Init ( loc, rightBottom, leftBottom );
	this->mPlanes [ TOP_PLANE ].Init ( loc, leftTop, rightTop );
	this->mPlanes [ LEFT_PLANE ].Init ( loc, leftBottom, leftTop );
	this->mPlanes [ RIGHT_PLANE ].Init ( loc, rightTop, rightBottom );
	
	this->UpdateAABB ();
}

//----------------------------------------------------------------//
// viewAxis and worldUP should be normalized.
// All frustum plane normals will face *into* of the frustum.
// Please pass in the stuff and things using RADIANS.
void USFrustum::Init ( USVec3D& loc, USVec3D& viewAxis, USVec3D& worldUp, float nearD, float farD, float hFOV, float vFOV ) {

	this->mLoc = loc;

	// USCamera3D relative x axis in world space
	USVec3D xAxis;
	xAxis.Cross ( viewAxis, worldUp );
	xAxis.Norm ();

	// USCamera3D relative y axis in world space
	USVec3D yAxis;
	yAxis.Cross ( xAxis, viewAxis );
	yAxis.Norm ();
	
	USVec3D planeNorm;
	USVec3D planeLoc;
	
	// Near plane
	planeLoc = loc;
	planeLoc.Offset ( viewAxis, nearD );
	
	planeNorm = viewAxis;
	
	mPlanes [ NEAR_PLANE ].Init ( planeLoc, planeNorm );
	
	// Far plane
	planeLoc = loc;
	planeLoc.Offset ( viewAxis, farD );

	planeNorm = viewAxis;
	planeNorm.Invert ();

	mPlanes [ FAR_PLANE ].Init ( planeLoc, planeNorm );
	
	// Left plane
	planeLoc = loc;

	planeNorm = xAxis;
	planeNorm.RotateInBasis ( xAxis, viewAxis, ( hFOV * 0.5f ));
	mPlanes [ LEFT_PLANE ].Init ( planeLoc, planeNorm );

	// Right plane
	planeNorm.Reflect ( xAxis );
	mPlanes [ RIGHT_PLANE ].Init ( planeLoc, planeNorm );

	// Top plane
	planeNorm = viewAxis;
	planeNorm.RotateInBasis ( viewAxis, yAxis, ( vFOV * 0.5f ) - ( 90.0f * ( float )D2R ));
	mPlanes [ TOP_PLANE ].Init ( planeLoc, planeNorm );

	// Bottom plane
	planeNorm.Reflect ( yAxis );
	mPlanes [ BOTTOM_PLANE ].Init ( planeLoc, planeNorm );
	
	this->UpdateAABB ();
}

//----------------------------------------------------------------//
void USFrustum::UpdateAABB () {

	USPlane3D& left = this->mPlanes [ LEFT_PLANE ];
	USPlane3D& right = this->mPlanes [ RIGHT_PLANE ];
	USPlane3D& top = this->mPlanes [ TOP_PLANE ];
	USPlane3D& bottom = this->mPlanes [ BOTTOM_PLANE ];
	USPlane3D& near = this->mPlanes [ NEAR_PLANE ];
	USPlane3D& far = this->mPlanes [ FAR_PLANE ];

	USVec3D leftBottom;
	USVec3D rightBottom;
	USVec3D rightTop;
	USVec3D leftTop;

	// Get the vecs to verts
	leftBottom.Cross ( bottom.mNorm, left.mNorm );
	rightBottom.Cross ( right.mNorm, bottom.mNorm );
	rightTop.Cross ( top.mNorm, right.mNorm );
	leftTop.Cross ( left.mNorm, top.mNorm );
	
	float t;
	
	// Intersect the vecs to verts to get the frustum corners
	USSect::VecToPlane ( this->mLoc, leftBottom, near, t, this->mPoints [ NEAR_LB_POINT ]);
	USSect::VecToPlane ( this->mLoc, leftBottom, far, t, this->mPoints [ FAR_LB_POINT ]);
	
	USSect::VecToPlane ( this->mLoc, rightBottom, near, t, this->mPoints [ NEAR_RB_POINT ]);
	USSect::VecToPlane ( this->mLoc, rightBottom, far, t, this->mPoints [ FAR_RB_POINT ]);
	
	USSect::VecToPlane ( this->mLoc, leftTop, near, t, this->mPoints [ NEAR_LT_POINT ]);
	USSect::VecToPlane ( this->mLoc, leftTop, far, t, this->mPoints [ FAR_LT_POINT ]);
	
	USSect::VecToPlane ( this->mLoc, rightTop, near, t, this->mPoints [ NEAR_RT_POINT ]);
	USSect::VecToPlane ( this->mLoc, rightTop, far, t, this->mPoints [ FAR_RT_POINT ]);
	
	// Compute the frustum's axis-aligned bounding box
	mAABB.Init ( mPoints [ 0 ]);
	mAABB.Grow ( mPoints [ 1 ]);
	mAABB.Grow ( mPoints [ 2 ]);
	mAABB.Grow ( mPoints [ 3 ]);
	mAABB.Grow ( mPoints [ 4 ]);
	mAABB.Grow ( mPoints [ 5 ]);
	mAABB.Grow ( mPoints [ 6 ]);
	mAABB.Grow ( mPoints [ 7 ]);
}

//----------------------------------------------------------------//
USFrustum::USFrustum () {
}
