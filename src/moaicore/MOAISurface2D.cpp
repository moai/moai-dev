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
bool MOAISurface2D::GetCircleContact ( USVec2D& sphereLoc, USVec2D& contact, USVec2D& norm ) const {

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
bool MOAISurface2D::GetCircleDepthAlongRay ( const USVec2D& loc, const USVec2D& ray, USVec2D& contact, float& time ) const {
	
	float t0, t1;
	u32 sectType;
	
	// make sure ray is headed in same direction as surface normal
	USVec2D r = ray;
	if ( this->mNorm.Dot ( ray ) < 0.0f ) {
		r.Reverse ();
	}
	
	// start with the point on the circle closest to the plane
	USVec2D pofcop = this->mNorm;
	pofcop.Reverse ();
	pofcop.Add ( loc );
	
	// intersect surface point with plane along ray
	sectType = USSect::VecToPlane ( pofcop, r, *this, t0 );
	if ( sectType == USSect::SECT_PARALLEL ) return false;
	
	// get the point of intersection
	pofcop.Add ( r, t0 );
	
	// see where it lies on the surface
	float edgeDist = this->mTangent.Dot ( pofcop );
	
	// if the point is outside of the surface, snap it to an edge
	if ( edgeDist < this->mP0 ) {
	
		USVec2D offset = this->mTangent;
		offset.Scale ( this->mP0 - edgeDist );
		pofcop.Add ( offset );
	}
	else if ( edgeDist > this->mP1 ) {
	
		USVec2D offset = this->mTangent;
		offset.Scale ( this->mP1 - edgeDist );
		pofcop.Add ( offset );
	}
	else {
	
		// point lies inside the surface - return the time of intersection
		time = -t0;
		contact = pofcop;
		return true;
	}
	
	// get the intersection from the edge to the circle
	sectType = USSect::VecToCircle ( t0, t1, pofcop, r, loc, 1.0f );

	// Bail if the point will not intersect the sphere.
	if ( sectType == USSect::SECT_PARALLEL ) return false;
	if ( sectType == USSect::SECT_TANGENT ) return false;

	time = t0;
	contact = pofcop;
	return true;
}

//----------------------------------------------------------------//
bool MOAISurface2D::GetCircleHit ( const USVec2D& loc, const USVec2D& move, float maxTime, MOAISurfaceHit2D& hit ) const {
	
	hit.mSurface = 0;
	
	if ( move.Dot ( this->mNorm ) > 0.0f ) return false;
	
	// The usual stuff...
	USVec2D unitMove = move;
	unitMove.Norm ();
	
	// Bail if center of circle is on or behind surface
	if ( USDist::PointToPlane2D ( loc, *this ) <= 0.0f ) return false;
	
	// Get the point of first contact on the polygon...
	USVec2D pofcop = this->mNorm;
	pofcop.Reverse ();
	pofcop.Add ( loc );
	this->ClampPoint ( pofcop );

	// Send a ray from the point on the surface to intersect the circle.
	// The ray is the inverse of the move vec.
	USVec2D inverseMove = move;
	inverseMove.Reverse ();

	float t0, t1;
	u32 sectType;
	sectType = USSect::VecToCircle ( t0, t1, pofcop, inverseMove, loc, 1.0f );

	// Bail if the point will not intersect the sphere.
	if ( sectType == USSect::SECT_PARALLEL ) return false;
	
	if ( t0 >= maxTime ) return false;;
	
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
	
	hit.mNorm = loc;
	hit.mNorm.Sub ( hit.mPoint );
	hit.mNorm.Norm ();
	
	if ( unitMove.Dot ( hit.mNorm ) > 0.0f ) return false;
	
	hit.mSurface = this;
	return true;
}

//----------------------------------------------------------------//
float MOAISurface2D::GetMinDistToEdge ( const USVec2D& loc ) const {

	float d = this->mTangent.Dot ( loc );
	float d0 = d - this->mP0;
	float d1 = this->mP1 - d;
	
	return ( d0 < d1 ) ? d0 : d1;
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
bool MOAISurface2D::GetMoveHit ( const USVec2D& loc, const USVec2D& move, float pad, float& time ) const {

	float d;
	d = move.Dot ( this->mNorm );
	if ( d == 0.0f ) return false;
	
	time = ( loc.Dot ( this->mNorm ) + this->mDist ) / -d;
	
	// now get the point of impact
	USVec2D sect = move;
	sect.Scale ( time );
	sect.Add ( loc );
	
	if ( move.mX > 0.0f ) {
		if ( sect.mX >= ( this->mXMax + pad )) {
			return false;
		}
	}
	
	if ( move.mX < 0.0f ) {
		if ( sect.mX <= ( this->mXMin - pad )) {
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------//
bool MOAISurface2D::GetRayHit ( const USVec2D& loc, const USVec2D& ray, float& time ) const {

	float d;
	d = ray.Dot ( this->mNorm );
	if ( d == 0.0f ) return false; // ray is parallel
	
	time = ( loc.Dot ( this->mNorm ) + this->mDist ) / -d;
	
	return true;
}

//----------------------------------------------------------------//
bool MOAISurface2D::GetRayHit ( const USVec2D& loc, const USVec2D& ray, float pad, float& time ) const {

	float d;
	d = ray.Dot ( this->mNorm );
	if ( d == 0.0f ) return false;
	
	time = ( loc.Dot ( this->mNorm ) + this->mDist ) / -d;
	
	// now get the point of impact
	USVec2D sect = ray;
	sect.Scale ( time );
	sect.Add ( loc );
	
	float dot = this->mTangent.Dot ( sect );
	
	if ( dot < ( this->mP0 - pad )) { time = dot; return false; }
	if ( dot > ( this->mP1 + pad )) { time = dot; return false; }
	
	return true;
}

//----------------------------------------------------------------//
float MOAISurface2D::GetShove ( const USVec2D& loc ) const {

	if ( this->mNorm.mX == 0.0f ) return 0.0f;
	
	// dist from loc to plane
	float dist = loc.Dot ( this->mNorm ) + this->mDist;
	
	// closest point on plane
	USVec2D pofcop = this->mNorm;
	pofcop.Scale ( -dist );
	pofcop.Add ( loc );
	
	// get dist along tangent
	USVec2D offset = this->mTangent;
	float edgeDist = offset.Dot ( pofcop );
	
	if ( edgeDist < this->mP0 ) {
		edgeDist = this->mP0 - edgeDist;
	}
	else if ( edgeDist > this->mP1 ) {
		edgeDist = this->mP1 - edgeDist;
	}
	else {
		// point is between edges - make a speedy exit
		if (( dist <= 0.0f ) || ( dist >= 1.0f )) return 0.0f;
		return (( 1.0f / this->mNorm.mX ) * dist ) * (( 1.0f - dist ) / dist);
	}
	
	offset.Scale ( edgeDist );
	pofcop.Add ( offset );
	
	// throw away if not inside the unit circle
	if ( loc.DistSqrd ( pofcop ) >= 1.0f ) return 0.0f;
	
	float a = pofcop.mY - loc.mY; // height of pofcop is sin (unit circle)
	float b = sqrtf ( 1.0f - ( a * a )); // convert to sin to consine
	
	// b (or cosine) is half of the width of the circle at height a
	
	if ( this->mNorm.mX > 0.0f ) {
		// surface is to the left of loc; shove right
		return b - ( loc.mX - pofcop.mX );
	}

	// surface is to the right of loc; shove left
	return -( b + ( loc.mX - pofcop.mX ));
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
void MOAISurface2D::Init ( MOAIProp* sourceProp, const USVec2D& v0, const USVec2D& v1 ) {

	this->mSourceProp = sourceProp;

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

////----------------------------------------------------------------//
//bool MOAISurface2D::IsBridge ( const USVec2D& loc, const USVec2D& move ) const {
//
//	USVec2D destLoc = loc;
//	destLoc.Add ( move );
//	
//	return ( this->IsOn ( destLoc ));
//}

//----------------------------------------------------------------//
bool MOAISurface2D::IsBridge ( const USVec2D& loc, const USVec2D& move, float pad, float& time ) const {

	float dist = USDist::PointToPlane2D ( loc, *this );
	if (( dist < pad ) || ( dist > -pad )) {
	
		float distance = move.Dot ( this->mTangent );
		if ( distance == 0.0f ) return false;
		
		float start = loc.Dot ( this->mTangent );
		float finish = start + distance;
		
		if ( distance > 0.0f ) {
			if (( start < this->mP1 ) && ( finish > this->mP0 )) {
				time = ( this->mP1 - start ) / distance;
				return true;
			}
		}
		else {
			if (( start > this->mP0 ) && ( finish < this->mP1 )) {
				time = ( this->mP0 - start ) / distance;
				return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAISurface2D::IsLeaving ( const USVec2D& loc, const USVec2D& move, float pad ) const {

	if ( move.mX > 0.0f ) {
		if ( loc.mX >= ( this->mXMax + pad )) {
			return true;
		}
	}
	
	if ( move.mX < 0.0f ) {
		if ( loc.mX <= ( this->mXMin - pad )) {
			return true;
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
//bool MOAISurface2D::IsOn ( const USVec2D& loc ) const {
//
//	if ( this->IsOver ( loc )) {
//
//		float dist = USDist::PointToPlane2D ( loc, *this );
//		return (( dist > -0.0001f ) && ( dist < 0.0001f ));
//	}
//	return false;
//}

//----------------------------------------------------------------//
bool MOAISurface2D::IsOnEdge ( const USVec2D& loc, float pad ) const {

	float d = this->mTangent.Dot ( loc );
	if (( d < ( this->mP0 + pad )) && ( d > ( this->mP0 - pad ))) return true;
	if (( d < ( this->mP1 + pad )) && ( d > ( this->mP1 - pad ))) return true;
	return false;
}

//----------------------------------------------------------------//
bool MOAISurface2D::IsOnPlane ( const USVec2D& loc, float pad ) const {

	float dist = USDist::PointToPlane2D ( loc, *this );
	return (( dist < pad ) || ( dist > -pad ));
}

////----------------------------------------------------------------//
//bool MOAISurface2D::IsOver ( const USVec2D& loc ) const {
//
//	float epsilon = 0.01f;
//
//	if ( loc.mX < ( this->mXMin - epsilon )) return false;
//	if ( loc.mX > ( this->mXMax + epsilon )) return false;
//
//	return true;
//}
