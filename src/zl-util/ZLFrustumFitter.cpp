// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLFrustumFitter.h>
#include <zl-util/ZLDistance.h>
#include <zl-util/ZLIntersect.h>
#include <zl-util/ZLMathConsts.h>

//================================================================//
// ZLFrustumFitter
//================================================================//

//----------------------------------------------------------------//
void ZLFrustumFitter::FitBox ( const ZLBox& box, float pad ) {

	for ( u32 i = 0; i < TOTAL_PLANES; ++i ) {
	
		ZLPlane3D& plane = this->mPlanes [ i ];
		
		ZLPlane3D originPlane = plane;
		originPlane.mDist = 0.0f;
		
		float radius;
		float dist = ZLDist::BoxToPlane ( box, originPlane, radius );
		
		dist -= radius + pad;
		
		if ( this->mNeedsInit || ( dist < -plane.mDist )) {
			plane.mDist = -dist;
			this->mIsDirty = true;
		}
	}
	this->mNeedsInit = false;
}

//----------------------------------------------------------------//
void ZLFrustumFitter::FitPoint ( const ZLVec3D& loc, float radius ) {

	for ( u32 i = 0; i < TOTAL_PLANES; ++i ) {
	
		ZLPlane3D& plane = this->mPlanes [ i ];
		
		float dist = plane.mNorm.Dot ( loc ) - radius;
		
		if ( this->mNeedsInit || ( dist < -plane.mDist )) {
			plane.mDist = -dist;
			this->mIsDirty = true;
		}
	}
	this->mNeedsInit = false;
}

//----------------------------------------------------------------//
ZLVec3D ZLFrustumFitter::GetPosition () {

	//orient.TransformVec ( this->mFitAxis );

	if ( this->mIsDirty ) {
		
		// set up the vertical plane that splits the view
		ZLVec3D vHalfPlanePos;
		ZLVec3D vHalfPlaneTan;
		ZLSect::PlaneToPlane ( this->mPlanes [ LEFT_PLANE ], this->mPlanes [ RIGHT_PLANE ], vHalfPlanePos, vHalfPlaneTan );
		vHalfPlaneTan.Norm ();

		// set up the horizontal plane that splits the view
		ZLVec3D hHalfPlanePos;
		ZLVec3D hHalfPlaneTan;
		ZLSect::PlaneToPlane ( this->mPlanes [ BOTTOM_PLANE ], this->mPlanes [ TOP_PLANE ], hHalfPlanePos, hHalfPlaneTan );
		hHalfPlaneTan.Norm ();
		
		ZLVec3D fitAxis = ZLVec3D::CrossNorm ( hHalfPlaneTan, vHalfPlaneTan );
		
		ZLPlane3D vHalfPlane;
		ZLPlane3D hHalfPlane;
		
		vHalfPlane.Init ( vHalfPlanePos, ZLVec3D::CrossNorm ( fitAxis, vHalfPlaneTan ));
		hHalfPlane.Init ( hHalfPlanePos, ZLVec3D::CrossNorm ( fitAxis, hHalfPlaneTan ));

		// calculate the eye plane
		ZLPlane3D eyePlane;
		eyePlane.Init ( hHalfPlanePos, fitAxis );
		
		float temp = ZLDist::VecToPlane ( vHalfPlanePos, eyePlane );
		if ( temp < 0.0f ) {
			eyePlane.mDist -= temp;
		}

		// The camera is at the intersection of the three planes we just found
		ZLVec3D pos;
		
		pos = eyePlane.mNorm;
		pos.Scale ( -( eyePlane.mDist ));

		pos.Sub ( vHalfPlane.mNorm, ZLDist::VecToPlane ( pos, vHalfPlane ));
		pos.Sub ( hHalfPlane.mNorm, ZLDist::VecToPlane ( pos, hHalfPlane ));
		
		this->mPosition = pos;
		this->mIsDirty = false;
	}
	
	return this->mPosition;
}

//----------------------------------------------------------------//
void ZLFrustumFitter::Init ( const ZLRect& viewRect, const ZLRect& fitRect, float hFov, const ZLAffine3D& orient ) {

	float hWidth = viewRect.Width () * 0.5f;
	float hHeight = viewRect.Height () * 0.5f;
	float distToNearPlane = hWidth * Cot ( hFov * 0.5f * ( float )D2R );

	float aspect = hWidth / ( viewRect.Height () * 0.5f );

	this->mIsDirty = false;
	this->mNeedsInit = true;

	this->mPosition.Init ( 0.0f, 0.0f, 0.0f );

    float t;
	
	// left and right frustum planes
	t = atanf (( hWidth - fitRect.mXMin ) / distToNearPlane );
    this->mPlanes [ LEFT_PLANE ].Init ( cosf ( t ), 0.0f, -sinf ( t ), 0.0f );
	
	t = atanf (( fitRect.mXMax - hWidth ) / distToNearPlane );
	this->mPlanes [ RIGHT_PLANE ].Init ( -cosf ( t ), 0.0f, -sinf ( t ), 0.0f );
	
    // bottom and top frustum planes
	t = atanf (( hHeight - fitRect.mYMin ) / distToNearPlane );
	this->mPlanes [ TOP_PLANE ].Init ( 0.0f, -cosf ( t ), -sinf ( t ), 0.0f );
	
	t = atanf (( fitRect.mYMax - hHeight ) / distToNearPlane );
	this->mPlanes [ BOTTOM_PLANE ].Init ( 0.0f, cosf ( t ), -sinf ( t ), 0.0f );
	
	for ( u32 i = 0; i < TOTAL_PLANES; ++i ) {
		orient.TransformVec ( this->mPlanes [ i ].mNorm ); // TODO: account for shear (use lighting matrix)
	}
}

//----------------------------------------------------------------//
void ZLFrustumFitter::Reset () {

	this->mNeedsInit = true;
}
