// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAISurface2D.h>

//================================================================//
// USSurfaceTouch2D
//================================================================//

////----------------------------------------------------------------//
//void USSurfaceTouch2D::Init ( USVec2D finger ) {
//
//	this->mTouch = false;
//	this->mPrevTouch = false;
//	this->mHit = false;
//	this->mFinger = finger;
//	this->mFinger.Norm ();
//}
//
////----------------------------------------------------------------//
//void USSurfaceTouch2D::Reset ( float dist ) {
//
//	this->mDist = dist;
//	
//	this->mPrevTouch = this->mTouch;
//	this->mTouch = false;
//	this->mHit = false;
//	this->mNorm.Init ( 0.0f, 0.0f );
//}
//
////----------------------------------------------------------------//
//void USSurfaceTouch2D::Scale ( float xSc, float ySc ) {
//
//	this->mPoint.Scale ( xSc, ySc );
//}

//================================================================//
// MOAISurface2D
//================================================================//

//----------------------------------------------------------------//
void MOAISurface2D::ClampPoint ( USVec2D& p ) const {

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
bool MOAISurface2D::GetContact ( USVec2D& sphereLoc, USVec2D& contact, USVec2D& norm ) const {

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
float MOAISurface2D::GetDepthAlongRay ( USVec2D& sphereLoc, USVec2D& ray ) const {
	
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
bool MOAISurface2D::GetHit ( USVec2D& sphereLoc, USVec2D& move, MOAISurfaceHit2D& hit ) const {

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

	// OMG! We hit something!

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
//USVec2D MOAISurface2D::GetNorm () {
//
//	USVec2D norm;
//
//	norm = this->mV0;
//	norm.Sub ( this->mV1 );
//	norm.Rotate90Anticlockwise ();
//	norm.Norm ();
//	
//	return norm;
//}

//----------------------------------------------------------------//
bool MOAISurface2D::GetRayHit ( USVec2D& loc, USVec2D& ray, float& time ) const {

	float d;
	d = ray.Dot ( this->mNorm );
	if ( d == 0.0f ) return false; // ray is parallel
	
	time = ( loc.Dot ( this->mNorm ) + this->mDist ) / -d;
	
	return true;
}

//----------------------------------------------------------------//
bool MOAISurface2D::GetRayHit ( USVec2D& loc, USVec2D& ray, float pad, float& time ) const {

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
//bool MOAISurface2D::GetTouch ( USVec2D& sphereLoc, USSurfaceTouch2D& touch ) {
//
//	// The usual stuff...
//	float dist = USDist::PointToPlane2D ( sphereLoc, *this );
//	if ( dist <= 0.0f ) return false;
//	if ( dist > 1.001f ) return false;
//	
//	// Get the point of first contact on the polygon...
//	USVec2D pofcop = this->mNorm;
//	pofcop.Reverse ();
//	pofcop.Add ( sphereLoc );
//	this->ClampPoint ( pofcop );
//
//	USVec2D ray = pofcop;
//	ray.Sub ( sphereLoc );
//	dist = ray.NormSafe ();
//	
//	if ( dist >= touch.mDist ) return false;
//
//	float dot = ray.Dot ( touch.mFinger );
//	if ( dot < 0.85f ) return false;
//
//	// we have a touch...
//	touch.mDist = dist;
//	touch.mPoint = pofcop;
//	touch.mTouch = true;
//	touch.mHit = !touch.mPrevTouch;
//	//touch.mNorm = this->GetNorm ();
//	
//	return true;
//}

//----------------------------------------------------------------//
void MOAISurface2D::Init ( const USVec2D& v0, const USVec2D& v1 ) {

	this->mV0 = v0;
	this->mV1 = v1;

	this->USPlane2D::Init ( v0, v1 );
	
	this->mTangent = this->mNorm;
	this->mTangent.Rotate90Anticlockwise ();
	
	this->mP0 = this->mTangent.Dot ( v0 );
	this->mP1 = this->mTangent.Dot ( v1 );
	
	if ( this->mP1 < this->mP0 ) {
		float p = this->mP0;
		this->mP0 = this->mP1;
		this->mP1 = p;
	}
	
	if ( v0.mX < v1.mX ) {
		this->mXMin = v0.mX;
		this->mXMax = v1.mX;
	}
	else {
		this->mXMin = v1.mX;
		this->mXMax = v0.mX;
	}
}

//----------------------------------------------------------------//
bool MOAISurface2D::IsBridge ( USVec2D& loc, USVec2D& move ) const {

	USVec2D destLoc = loc;
	destLoc.Add ( move );
	
	return ( this->IsOn ( destLoc ));
}

//----------------------------------------------------------------//
bool MOAISurface2D::IsLeaving ( USVec2D& loc, USVec2D& move ) const {

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
bool MOAISurface2D::IsOn ( USVec2D& loc ) const {

	if ( this->IsOver ( loc )) {

		float dist = USDist::PointToPlane2D ( loc, *this );
		return (( dist > -0.0001f ) && ( dist < 0.0001f ));
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAISurface2D::IsOver ( USVec2D& loc ) const {

	float epsilon = 0.01f;

	if ( loc.mX < ( this->mXMin - epsilon )) return false;
	if ( loc.mX > ( this->mXMax + epsilon )) return false;

	return true;
}
