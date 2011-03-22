// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USFrustum.h>
#include <uslsext/USDistance.h>
#include <uslsext/USIntersect.h>
#include <uslsext/USMathConsts.h>

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

////----------------------------------------------------------------//
//void USFrustum::Fatten ( USVec3D& loc ) {
//
//	this->FattenPlane ( this->mPlanes [ LEFT_PLANE ], loc );
//	this->FattenPlane ( this->mPlanes [ RIGHT_PLANE ], loc );
//	this->FattenPlane ( this->mPlanes [ TOP_PLANE ], loc );
//	this->FattenPlane ( this->mPlanes [ BOTTOM_PLANE ], loc );
//}
//
////----------------------------------------------------------------//
//void USFrustum::Fatten ( USRhombus& rhombus ) {
//
//	USVec3D loc = rhombus.mLoc;
//	this->Fatten ( loc );
//	
//	loc.Add ( rhombus.mXAxis );
//	this->Fatten ( loc );
//	
//	loc = rhombus.mLoc;
//	loc.Add ( rhombus.mYAxis );
//	this->Fatten ( loc );
//	
//	loc.Add ( rhombus.mXAxis );
//	this->Fatten ( loc );
//}
//
////----------------------------------------------------------------//
//// This method first checks to see if the point is behind the plane.
//// If it is, the plane is moved to touch the point.
//void USFrustum::FattenPlane ( USPlane3D& plane, USVec3D& loc ) {
//
//	USPlane3D& front = this->mPlanes [ NEAR_PLANE ];
//
//	// Do nothing if loc is in behind the front plane.
//	if ( USDist::VecToPlane ( loc, front ) <= 0.0f ) return;
//
//	// Do nothing if loc is in front of plane in question.
//	if ( USDist::VecToPlane ( loc, plane ) >= 0.0f ) return;
//
//	// Loc is behind plane, so we need to move the plane to touch the loc.
//	
//	// First, get the normal perpindicular to the plane.
//	USVec3D xAxis;
//	xAxis.Cross ( front.mNorm, plane.mNorm );
//	
//	// Now get a normal looking at the point, which will lie on the new plane.
//	USVec3D zAxis = loc;
//	zAxis.Sub ( this->mLoc );
//	zAxis.Norm ();
//	
//	// Finally, figure the normal of the new plane and initialize the plane.
//	USVec3D norm;
//	norm.Cross ( zAxis, xAxis );
//	plane.Init ( this->mLoc, norm );
//	
//	this->mIsValid = false;
//}

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
	
	this->UpdateHull ();
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
	
	this->UpdateHull ();
}

//----------------------------------------------------------------//
void USFrustum::UpdateHull () {

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
	
	// Intersect the vecs to verts to get the frustum corners
	USSect::VecToPlane ( this->mLoc, leftBottom, near, this->mPoints [ NEAR_LB_POINT ]);
	USSect::VecToPlane ( this->mLoc, leftBottom, far, this->mPoints [ FAR_LB_POINT ]);
	
	USSect::VecToPlane ( this->mLoc, rightBottom, near, this->mPoints [ NEAR_RB_POINT ]);
	USSect::VecToPlane ( this->mLoc, rightBottom, far, this->mPoints [ FAR_RB_POINT ]);
	
	USSect::VecToPlane ( this->mLoc, leftTop, near, this->mPoints [ NEAR_LT_POINT ]);
	USSect::VecToPlane ( this->mLoc, leftTop, far, this->mPoints [ FAR_LT_POINT ]);
	
	USSect::VecToPlane ( this->mLoc, rightTop, near, this->mPoints [ NEAR_RT_POINT ]);
	USSect::VecToPlane ( this->mLoc, rightTop, far, this->mPoints [ FAR_RT_POINT ]);
	
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

//================================================================//
// USFrustShrinker
//================================================================//

//----------------------------------------------------------------//
void USFrustShrinker::Evaluate ( USVec3D& vec ) {

	USPlane3D& left		= this->mFrustum->mPlanes [ USFrustum::LEFT_PLANE ];
	USPlane3D& right	= this->mFrustum->mPlanes [ USFrustum::RIGHT_PLANE ];
	USPlane3D& top		= this->mFrustum->mPlanes [ USFrustum::TOP_PLANE ];
	USPlane3D& bottom	= this->mFrustum->mPlanes [ USFrustum::BOTTOM_PLANE ];
	USPlane3D& near		= this->mFrustum->mPlanes [ USFrustum::NEAR_PLANE ];

	USVec3D ray = vec;
	ray.Sub ( this->mFrustum->mLoc );

	USVec3D projectedVec;
	USSect::VecToPlane ( this->mFrustum->mLoc, ray, near, projectedVec);

	if ( this->mTotalPoints ) {
	
		float dist;
		
		dist = USDist::VecToPlane ( projectedVec, left );
		if ( dist < this->mBestDist [ USFrustum::LEFT_PLANE ] ) {
			this->mBestDist [ USFrustum::LEFT_PLANE ] = dist;
			this->mBestLoc [ USFrustum::LEFT_PLANE ] = projectedVec;
		}
		
		dist = USDist::VecToPlane ( projectedVec, right );
		if ( dist < this->mBestDist [ USFrustum::RIGHT_PLANE ] ) {
			this->mBestDist [ USFrustum::RIGHT_PLANE ] = dist;
			this->mBestLoc [ USFrustum::RIGHT_PLANE ] = projectedVec;
		}
		
		dist = USDist::VecToPlane ( projectedVec, top );
		if ( dist < this->mBestDist [ USFrustum::TOP_PLANE ] ) {
			this->mBestDist [ USFrustum::TOP_PLANE ] = dist;
			this->mBestLoc [ USFrustum::TOP_PLANE ] = projectedVec;
		}
		
		dist = USDist::VecToPlane ( projectedVec, bottom );
		if ( dist < this->mBestDist [ USFrustum::BOTTOM_PLANE ] ) {
			this->mBestDist [ USFrustum::BOTTOM_PLANE ] = dist;
			this->mBestLoc [ USFrustum::BOTTOM_PLANE ] = projectedVec;
		}
	}
	else {
	
		this->mBestDist [ USFrustum::LEFT_PLANE ] = USDist::VecToPlane ( projectedVec, left );
		this->mBestLoc [ USFrustum::LEFT_PLANE ] = projectedVec;
		
		this->mBestDist [ USFrustum::RIGHT_PLANE ] = USDist::VecToPlane ( projectedVec, right );
		this->mBestLoc [ USFrustum::RIGHT_PLANE ] = projectedVec;
		
		this->mBestDist [ USFrustum::TOP_PLANE ] = USDist::VecToPlane ( projectedVec, top );
		this->mBestLoc [ USFrustum::TOP_PLANE ] = projectedVec;
		
		this->mBestDist [ USFrustum::BOTTOM_PLANE ] = USDist::VecToPlane ( projectedVec, bottom );
		this->mBestLoc [ USFrustum::BOTTOM_PLANE ] = projectedVec;
	}
	
	++this->mTotalPoints;
}

//----------------------------------------------------------------//
void USFrustShrinker::Evaluate ( USRhombus& rhombus ) {

	USVec3D vec = rhombus.mLoc;
	this->Evaluate ( vec );
	
	vec.Add ( rhombus.mXAxis );
	this->Evaluate ( vec );
	
	vec = rhombus.mLoc;
	vec.Add ( rhombus.mYAxis );
	this->Evaluate ( vec );
	
	vec.Add ( rhombus.mXAxis );
	this->Evaluate ( vec );
}

//----------------------------------------------------------------//
void USFrustShrinker::Reset () {

	this->mTotalPoints = 0;
}

//----------------------------------------------------------------//
void USFrustShrinker::ShrinkFrustum () {

	if ( this->mTotalPoints ) {
		this->ShrinkPlane ( USFrustum::LEFT_PLANE );
		this->ShrinkPlane ( USFrustum::RIGHT_PLANE );
		this->ShrinkPlane ( USFrustum::TOP_PLANE );
		this->ShrinkPlane ( USFrustum::BOTTOM_PLANE );
		
		this->mFrustum->UpdateHull ();
	}
}

//----------------------------------------------------------------//
void USFrustShrinker::ShrinkPlane ( u32 id ) {

	if ( this->mBestDist [ id ] <= 0.0f ) return;

	USPlane3D& plane = this->mFrustum->mPlanes [ id ];
	USPlane3D& front = this->mFrustum->mPlanes [ USFrustum::NEAR_PLANE ];
	
	// First, get the normal perpindicular to the plane.
	USVec3D xAxis;
	xAxis.Cross ( front.mNorm, plane.mNorm );
	
	// Now get a normal looking at the point, which will lie on the new plane.
	USVec3D zAxis = this->mBestLoc [ id ];
	zAxis.Sub ( this->mFrustum->mLoc );
	zAxis.Norm ();
	
	// Finally, figure the normal of the new plane and initialize the plane.
	USVec3D norm;
	norm.Cross ( xAxis, zAxis );
	plane.Init ( this->mFrustum->mLoc, norm );
}

//----------------------------------------------------------------//
USFrustShrinker::USFrustShrinker () {
}

//----------------------------------------------------------------//
USFrustShrinker::USFrustShrinker ( USFrustum& frustum ) :
	mFrustum ( &frustum ),
	mTotalPoints ( 0 ) {
}

//================================================================//
// USFrustCone
//================================================================//

//----------------------------------------------------------------//
bool USFrustCone::Cull ( USBox& box ) {

	// No luck, so try to cull the box by the planes of the frustum
	if ( USSect::BoxToPlane ( box, mPlanes [ LEFT_PLANE ]) < 0 ) return true;
	if ( USSect::BoxToPlane ( box, mPlanes [ RIGHT_PLANE ]) < 0 ) return true;
	if ( USSect::BoxToPlane ( box, mPlanes [ TOP_PLANE ]) < 0 ) return true;
	if ( USSect::BoxToPlane ( box, mPlanes [ BOTTOM_PLANE ]) < 0 ) return true;

	// At this point it looks like we can't cull, so return false
	return false;
}

//----------------------------------------------------------------//
bool USFrustCone::Cull ( USPrism& prism ) {
	
	// No luck, so try to cull the box by the planes of the frustum
	if ( USSect::PrismToPlane ( prism, mPlanes [ LEFT_PLANE ]) < 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ RIGHT_PLANE ]) < 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ TOP_PLANE ]) < 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ BOTTOM_PLANE ]) < 0 ) return true;

	// At this point it looks like we can't cull, so return false
	return false;
}

//----------------------------------------------------------------//
bool USFrustCone::Cull ( USRhombus& rhombus ) {
	
	// No luck, so try to cull the box by the planes of the frustum
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ LEFT_PLANE ]) < 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ RIGHT_PLANE ]) < 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ TOP_PLANE ]) < 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ BOTTOM_PLANE ]) < 0 ) return true;

	// At this point it looks like we can't cull, so return false
	return false;
}

//----------------------------------------------------------------//
bool USFrustCone::CullConvex ( USVec3D* convex, u32 total  ) {
	UNUSED ( convex );
	UNUSED ( total );

	// At this point it looks like we can't cull, so return false
	return false;
}

//----------------------------------------------------------------//
void  USFrustCone::Init ( USFrustum& frustum ) {

	this->mLoc = frustum.mLoc;
	
	this->mPlanes [ LEFT_PLANE ] = frustum.mPlanes [ USFrustum::LEFT_PLANE ];
	this->mPlanes [ RIGHT_PLANE ] = frustum.mPlanes [ USFrustum::RIGHT_PLANE ];
	this->mPlanes [ BOTTOM_PLANE ] = frustum.mPlanes [ USFrustum::BOTTOM_PLANE ];
	this->mPlanes [ TOP_PLANE ] = frustum.mPlanes [ USFrustum::TOP_PLANE ];
}

//----------------------------------------------------------------//
// Horrible, horrible...
//void USFrustCone::Init ( USVec3D& loc, USRhombus& rhombus ) {
//
//	this->mLoc = loc;
//
//	// Get the plane containing the rhombus
//	USPlane3D rhomPlane;
//	rhombus.GetPlane ( rhomPlane );
//	
//	// Bless the rhombus
//	USRhombus blessedRhombus = rhombus;
//	if ( USDist::VecToPlane ( loc, rhomPlane ) < 0.0f ) {
//	
//		// We're behind the rhombus plane; the plane's normal is facing away from us
//		// So flip the plane normal as well as the rhombus' Y axis
//		rhomPlane.mNorm.Invert ();
//		blessedRhombus.mLoc.Add ( blessedRhombus.mYAxis );
//		blessedRhombus.mYAxis.Invert ();
//	}
//
//	// Now all our calculations will be correct!
//	// And all your base are belong to us!  Hooray! 
//
//	// Get vertices of rhombus
//	USVec3D leftBottom = blessedRhombus.mLoc;
//	
//	USVec3D rightBottom = leftBottom;
//	rightBottom.Add ( blessedRhombus.mXAxis );
//	
//	USVec3D rightTop = rightBottom;
//	rightTop.Add ( blessedRhombus.mYAxis );
//	
//	USVec3D leftTop = rightTop;
//	leftTop.Sub ( blessedRhombus.mXAxis );
//	
//	// Init the side planes
//	this->mPlanes [ BOTTOM_PLANE ].Init ( loc, rightBottom, leftBottom );
//	this->mPlanes [ TOP_PLANE ].Init ( loc, leftTop, rightTop );
//	this->mPlanes [ LEFT_PLANE ].Init ( loc, leftBottom, leftTop );
//	this->mPlanes [ RIGHT_PLANE ].Init ( loc, rightTop, rightBottom );
//}

//----------------------------------------------------------------//
// viewAxis and worldUP should be normalized.
// All frustum plane normals will face *into* of the frustum.
// Please pass in the stuff and things using RADIANS.
void USFrustCone::Init ( USVec3D& loc, USVec3D& viewAxis, USVec3D& worldUp, float hFOV, float vFOV ) {

	// USCamera3D relative x axis in world space
	USVec3D xAxis;
	xAxis.Cross ( viewAxis, worldUp );
	xAxis.Norm ();

	// USCamera3D relative y axis in world space
	USVec3D yAxis;
	yAxis.Cross ( xAxis, viewAxis );
	yAxis.Norm ();
	
	USVec3D planeNorm;
	
	// Left plane
	planeNorm = xAxis;
	planeNorm.RotateInBasis ( xAxis, viewAxis, ( hFOV * 0.5f ));
	mPlanes [ LEFT_PLANE ].Init ( loc, planeNorm );

	// Right plane
	planeNorm.Reflect ( xAxis );
	mPlanes [ RIGHT_PLANE ].Init ( loc, planeNorm );

	// Top plane
	planeNorm = viewAxis;
	planeNorm.RotateInBasis ( viewAxis, yAxis, ( vFOV * 0.5f ) - ( 90.0f * ( float )D2R ));
	mPlanes [ TOP_PLANE ].Init ( loc, planeNorm );

	// Bottom plane
	planeNorm.Reflect ( yAxis );
	mPlanes [ BOTTOM_PLANE ].Init ( loc, planeNorm );
}

//================================================================//
// USFrustboxum
//================================================================//

//----------------------------------------------------------------//
bool USFrustboxum::Cull ( USBox& box ) {

	// Try to casually cull the frustum by the extents of the box
	// i.e. we can bail if the frustum AABB doesn't overlap the box
	if ( !box.Overlap ( mAABB )) return true;
	return this->mFrustCone.Cull ( box );
}
