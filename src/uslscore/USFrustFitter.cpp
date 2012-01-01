// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USFrustFitter.h>
#include <uslscore/USFrustum.h>
#include <uslscore/USDistance.h>
#include <uslscore/USIntersect.h>
//#include <uslscore/USMathConsts.h>
//#include <uslscore/USPrism.h>
#include <uslscore/USRhombus.h>

//================================================================//
// USFrustFitter
//================================================================//

//----------------------------------------------------------------//
void USFrustFitter::Evaluate ( USVec3D& vec ) {

	USPlane3D& left		= this->mFrustum->mPlanes [ USFrustum::LEFT_PLANE ];
	USPlane3D& right	= this->mFrustum->mPlanes [ USFrustum::RIGHT_PLANE ];
	USPlane3D& top		= this->mFrustum->mPlanes [ USFrustum::TOP_PLANE ];
	USPlane3D& bottom	= this->mFrustum->mPlanes [ USFrustum::BOTTOM_PLANE ];
	USPlane3D& near		= this->mFrustum->mPlanes [ USFrustum::NEAR_PLANE ];

	USVec3D ray = vec;
	ray.Sub ( this->mFrustum->mLoc );
	
	float t;
	
	USVec3D projectedVec;
	USSect::VecToPlane ( this->mFrustum->mLoc, ray, near, t, projectedVec );

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
void USFrustFitter::Evaluate ( USRhombus& rhombus ) {

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
void USFrustFitter::Reset () {

	this->mTotalPoints = 0;
}

//----------------------------------------------------------------//
void USFrustFitter::ShrinkFrustum () {

	if ( this->mTotalPoints ) {
		this->ShrinkPlane ( USFrustum::LEFT_PLANE );
		this->ShrinkPlane ( USFrustum::RIGHT_PLANE );
		this->ShrinkPlane ( USFrustum::TOP_PLANE );
		this->ShrinkPlane ( USFrustum::BOTTOM_PLANE );
		
		this->mFrustum->UpdateAABB ();
	}
}

//----------------------------------------------------------------//
void USFrustFitter::ShrinkPlane ( u32 id ) {

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
USFrustFitter::USFrustFitter () {
}

//----------------------------------------------------------------//
USFrustFitter::USFrustFitter ( USFrustum& frustum ) :
	mFrustum ( &frustum ),
	mTotalPoints ( 0 ) {
}
