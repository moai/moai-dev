// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef SURFACE2D_H
#define SURFACE2D_H

#include <uslscore/USMatrix3x3.h>
#include <uslscore/USPlane.h>

class USCollPrim2D;
class SurfaceHit2D;
class SurfaceSnap2D;

//================================================================//
// USSurface2D
//================================================================//
class USEdge2D {
public:
	USVec2D		mV0;
	USVec2D		mV1;
};

//================================================================//
// USSurfaceTouch2D
//================================================================//
class USSurfaceTouch2D {
public:

	// input
	USVec2D		mFinger; // touch vector (unit vector)

	// internal
	float		mDist;
	bool		mTouch;
	bool		mPrevTouch;
	bool		mHit;

	// output
	USVec2D		mPoint; // touch point
	USVec2D		mNorm;
	
	//----------------------------------------------------------------//
	void		Init			( USVec2D finger );
	void		Reset			( float dist = 1.25f );
	void		Scale			( float xSc, float ySc );
};

//================================================================//
// USSurface2D
//================================================================//
class USSurface2D :
	public USPlane2D {
public:

	USVec2D			mTangent; // perp to norm
	
	float			mP0;
	float			mP1;
	
	float			mXMin;
	float			mXMax;
	
	//----------------------------------------------------------------//
	void			ClampPoint			( USVec2D& p );
	static void		DrawDebug			( USVec2D e0, USVec2D e1 );
	bool			GetContact			( USVec2D& sphereLoc, USVec2D& contact, USVec2D& norm );
	float			GetDepthAlongRay	( USVec2D& sphereLoc, USVec2D& ray );
	bool			GetHit				( USVec2D& sphereLoc, USVec2D& move, SurfaceHit2D& hit ); // unit sphere
	static USVec2D	GetNorm				( const USVec2D& e0, const USVec2D& e1 );
	bool			GetRayHit			( USVec2D& loc, USVec2D& ray, float& time ); // ray, no edges
	bool			GetRayHit			( USVec2D& loc, USVec2D& ray, float pad, float& time ); // ray
	void			GetSnapUp			( USVec2D& loc, float maxSnap, USVec2D& move, SurfaceSnap2D& snap );
	bool			GetTouch			( USVec2D& sphereLoc, USSurfaceTouch2D& touch ); // unit sphere
	void			Init				( const USVec2D& e0, const USVec2D& e1 );
	bool			IsBridge			( USVec2D& loc, USVec2D& move );
	bool			IsLeaving			( USVec2D& loc, USVec2D& move );
	bool			IsOn				( USVec2D& loc );
	bool			IsOver				( USVec2D& loc );
};

//================================================================//
// SurfaceHit2D
//================================================================//
class SurfaceHit2D {
public:

	float		mTime;
	USVec2D		mPoint;
	USVec2D		mNorm;
};

//================================================================//
// SurfaceSnap2D
//================================================================//
class SurfaceSnap2D {
public:

	bool		mSnap;
	float		mSnapDist;
	USSurface2D	mSurface;
};

#endif
