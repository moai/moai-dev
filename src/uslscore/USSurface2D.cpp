// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USDistance.h>
#include <uslscore/USIntersect.h>
#include <uslscore/USSurface2D.h>

//================================================================//
// USSurfaceTouch2D
//================================================================//

//----------------------------------------------------------------//
void USSurfaceTouch2D::Init ( USVec2D finger ) {

	this->mTouch = false;
	this->mPrevTouch = false;
	this->mHit = false;
	this->mFinger = finger;
	this->mFinger.Norm ();
}

//----------------------------------------------------------------//
void USSurfaceTouch2D::Reset ( float dist ) {

	this->mDist = dist;
	
	this->mPrevTouch = this->mTouch;
	this->mTouch = false;
	this->mHit = false;
	this->mNorm.Init ( 0.0f, 0.0f );
}

//----------------------------------------------------------------//
void USSurfaceTouch2D::Scale ( float xSc, float ySc ) {

	this->mPoint.Scale ( xSc, ySc );
}

//================================================================//
// USSurface2D
//================================================================//

//----------------------------------------------------------------//
void USSurface2D::ClampPoint ( USVec2D& p ) {

	USDist::SnapToPlane2D ( p, *this );
	
	float edgeDist = this->mTangent.Dot ( p );
	
	if ( edgeDist < this->mP0 ) {
	
		USVec2D offset = this->mTangent;
		offset.Scale ( this->mP0 - edgeDist );
		p.Add ( offset );
	}
	else if ( edgeDist > this->mP1 ) {
	
		USVec2D offset = this->mTangent;
		offset.Scale ( this->mP1 - edgeDist );
		p.Add ( offset );
	}
}

//----------------------------------------------------------------//
void USSurface2D::DrawDebug ( USVec2D e0, USVec2D e1 ) {
	UNUSED ( e0 );
	UNUSED ( e1 );

	//USDebugHUD& debugHUD = USDebugHUD::Get ();
	//
	//debugHUD.DrawLine ( e0, e1, 0xffffffff, 2 );
	//
	//USVec2D mid = e1;
	//mid.Sub ( e0 );
	//mid.Scale ( 0.5f );
	//mid.Add ( e0 );
	//
	//USVec2D norm = GetNorm ( e0, e1 );
	//norm.Scale ( 0.25f );
	//norm.Add ( mid );
	//
	//debugHUD.DrawLine ( mid, norm, 0xffffffff, 1 );
}

//----------------------------------------------------------------//
bool USSurface2D::GetContact ( USVec2D& sphereLoc, USVec2D& contact, USVec2D& norm ) {

	// The usual stuff...
	float dist = USDist::PointToPlane2D ( sphereLoc, *this );
	if ( dist <= 0.0f ) return false;
	if ( dist > 1.001f ) return false;
	
	// Get the point of first contact on the polygon...
	contact = this->mNorm;
	contact.Reverse ();
	contact.Add ( sphereLoc );
	this->ClampPoint ( contact );

	norm = sphereLoc;
	norm.Sub ( contact );
	dist = norm.NormSafe ();
	
	if ( dist > 1.001f ) return false;
	return true;
}

//----------------------------------------------------------------//
float USSurface2D::GetDepthAlongRay ( USVec2D& sphereLoc, USVec2D& ray ) {
	
	// Get the point of first contact on the polygon...
	USVec2D pofcop = this->mNorm;
	pofcop.Reverse ();
	pofcop.Add ( sphereLoc );
	this->ClampPoint ( pofcop );

	float t0, t1;
	u32 sectType;
	sectType = USSect::VecToCircle ( t0, t1, pofcop, ray, sphereLoc, 1.0f );

	// Bail if the point will not intersect the sphere.
	if ( sectType == USSect::SECT_PARALLEL ) return 0.0f;
	if ( sectType == USSect::SECT_TANGENT ) return 0.0f;

	return t0;
}

//----------------------------------------------------------------//
bool USSurface2D::GetHit ( USVec2D& sphereLoc, USVec2D& move, SurfaceHit2D& hit ) {

	// The usual stuff...
	USVec2D unitMove = move;
	unitMove.Norm ();
	
	if ( unitMove.Dot ( this->mNorm ) >= -0.001f ) return false;
	if ( USDist::PointToPlane2D ( sphereLoc, *this ) <= 0.0f ) return false;
	
	// Get the point of first contact on the polygon...
	USVec2D pofcop = this->mNorm;
	pofcop.Reverse ();
	pofcop.Add ( sphereLoc );
	this->ClampPoint ( pofcop );

	// Send a ray from the point on the surface to intersect the circle.
	// The ray is the inverse of the move vec.
	USVec2D inverseMove = move;
	inverseMove.Reverse ();

	float t0, t1;
	u32 sectType;
	sectType = USSect::VecToCircle ( t0, t1, pofcop, inverseMove, sphereLoc, 1.0f );

	// Bail if the point will not intersect the sphere.
	if ( sectType == USSect::SECT_PARALLEL ) return false;

	if ( t0 >= hit.mTime ) return false;

	// Bail if the point will graze the sphere.
	if ( sectType == USSect::SECT_TANGENT ) return false;

	// Bail if the point will stay outside of the sphere.
	if (( t0 > 1.0f ) || ( t1 < 0.0f )) return false;

	// OMG!  We hit something!

	float time = t0;
	
	hit.mTime = time;

	inverseMove.Scale ( t0 );
	hit.mPoint = pofcop;
	hit.mPoint.Add ( inverseMove );
	
	hit.mNorm = sphereLoc;
	hit.mNorm.Sub ( hit.mPoint );
	hit.mNorm.Norm ();
	
	if ( unitMove.Dot ( hit.mNorm ) >= -0.001f ) return false;
	
	return true;
}

//----------------------------------------------------------------//
USVec2D USSurface2D::GetNorm ( const USVec2D& e0, const USVec2D& e1 ) {

	USVec2D norm;

	norm = e0;
	norm.Sub ( e1 );
	norm.Rotate90Anticlockwise ();
	norm.Norm ();
	
	return norm;
}

//----------------------------------------------------------------//
bool USSurface2D::GetRayHit ( USVec2D& loc, USVec2D& ray, float& time ) {

	float d;
	d = ray.Dot ( this->mNorm );
	if ( d == 0.0f ) return false; // ray is parallel
	
	time = ( loc.Dot ( this->mNorm ) + this->mDist ) / -d;
	
	return true;
}

//----------------------------------------------------------------//
bool USSurface2D::GetRayHit ( USVec2D& loc, USVec2D& ray, float pad, float& time ) {

	float d;
	d = ray.Dot ( this->mNorm );
	if ( d == 0.0f ) return false;
	
	time = ( loc.Dot ( this->mNorm ) + this->mDist ) / -d;
	
	// now get the point of impact
	USVec2D sect = ray;
	sect.Scale ( time );
	sect.Add ( loc );
	
	float dot = this->mTangent.Dot ( sect );
	
	if ( dot < ( this->mP0 - pad )) return false;
	if ( dot > ( this->mP1 + pad )) return false;
	
	return true;
}

//----------------------------------------------------------------//
void USSurface2D::GetSnapUp ( USVec2D& loc, float maxSnap, USVec2D& move, SurfaceSnap2D& snap ) {

	if ( this->mNorm.mY < 0.0f ) return;
	//if ( !this->IsOver ( loc )) return;

	//if ( this->IsLeaving ( loc, move )) return;

	USVec2D up ( 0.0f, 1.0f );
	float snapDist = 0.0f; // dist will be returned here
	if ( this->GetRayHit ( loc, up, snapDist ))  {
		
		if ( snapDist > maxSnap ) return; // return if snap is above us
		if ( snapDist < snap.mSnapDist ) return; // return if snap is lower than last best snap
		
		// 'snap' is true if we already have a valid snap
		if ( snap.mSnap ) {
			
			// looks like we have multiple floors to choose from...
			if ( snapDist > snap.mSnapDist ) {
			
				// we have a clear winner
				snap.mSurface = *this;
				snap.mSnapDist = snapDist;
			}
			else {
				
				// snap is the same as the last snap...
				
				// break the tie
				// if there's a move, choose the surface with the steepest angle
				// (against the move)
				if ( move.mX > 0.0f ) {
					if ( this->mNorm.mX < snap.mSurface.mNorm.mX ) {
						snap.mSurface = *this;
					}
				}
				else if ( move.mX < 0.0f ) {
					if ( this->mNorm.mX > snap.mSurface.mNorm.mX ) {
						snap.mSurface = *this;
					}
				}
				//else {
				//	if ( this->mNorm.mY < snap.mSurface.mNorm.mY ) {
				//		snap.mSurface = *this;
				//	}
				//}
			}
		}
		else {
		
			// first floor, so go with it
			snap.mSurface = *this;
			snap.mSnapDist = snapDist;
			snap.mSnap = true;
		}
		
		if ( snapDist > snap.mSnapDist ) {
			snap.mSnapDist = snapDist;
		}
	}
}

//----------------------------------------------------------------//
bool USSurface2D::GetTouch ( USVec2D& sphereLoc, USSurfaceTouch2D& touch ) {

	// The usual stuff...
	float dist = USDist::PointToPlane2D ( sphereLoc, *this );
	if ( dist <= 0.0f ) return false;
	if ( dist > 1.001f ) return false;
	
	// Get the point of first contact on the polygon...
	USVec2D pofcop = this->mNorm;
	pofcop.Reverse ();
	pofcop.Add ( sphereLoc );
	this->ClampPoint ( pofcop );

	USVec2D ray = pofcop;
	ray.Sub ( sphereLoc );
	dist = ray.NormSafe ();
	
	if ( dist >= touch.mDist ) return false;

	float dot = ray.Dot ( touch.mFinger );
	if ( dot < 0.85f ) return false;

	// we have a touch...
	touch.mDist = dist;
	touch.mPoint = pofcop;
	touch.mTouch = true;
	touch.mHit = !touch.mPrevTouch;
	//touch.mNorm = this->GetNorm ();
	
	return true;
}

//----------------------------------------------------------------//
void USSurface2D::Init ( const USVec2D& e0, const USVec2D& e1 ) {

	USVec2D worldNorm = this->GetNorm ( e0, e1 );

	this->USPlane2D::Init ( e0, e1 );
	
	this->mTangent = this->mNorm;
	this->mTangent.Rotate90Anticlockwise ();
	
	this->mP0 = this->mTangent.Dot ( e0 );
	this->mP1 = this->mTangent.Dot ( e1 );
	
	if ( this->mP1 < this->mP0 ) {
		float p = this->mP0;
		this->mP0 = this->mP1;
		this->mP1 = p;
	}
	
	if ( e0.mX < e1.mX ) {
		this->mXMin = e0.mX;
		this->mXMax = e1.mX;
	}
	else {
		this->mXMin = e1.mX;
		this->mXMax = e0.mX;
	}
}

//----------------------------------------------------------------//
bool USSurface2D::IsBridge ( USVec2D& loc, USVec2D& move ) {

	USVec2D destLoc = loc;
	destLoc.Add ( move );
	
	return ( this->IsOn ( destLoc ));
}

//----------------------------------------------------------------//
bool USSurface2D::IsLeaving ( USVec2D& loc, USVec2D& move ) {

	if ( move.mX > 0.0f ) {
		if ( loc.mX >= ( this->mXMax - 0.001f )) {
			return true;
		}
	}
	
	if ( move.mX < 0.0f ) {
		if ( loc.mX <= ( this->mXMin + 0.001f )) {
			return true;
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
bool USSurface2D::IsOn ( USVec2D& loc ) {

	if ( this->IsOver ( loc )) {

		float dist = USDist::PointToPlane2D ( loc, *this );
		return (( dist > -0.0001f ) && ( dist < 0.0001f ));
	}
	return false;
}

//----------------------------------------------------------------//
bool USSurface2D::IsOver ( USVec2D& loc ) {

	float epsilon = 0.01f;

	if ( loc.mX < ( this->mXMin - epsilon )) return false;
	if ( loc.mX > ( this->mXMax + epsilon )) return false;

	return true;
}
