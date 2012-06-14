// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MATH_PLANE_H
#define	MATH_PLANE_H

#include <uslscore/USVec2D.h>

//================================================================//
// USPlane2D
//================================================================//
class USPlane2D {
	public:

	USVec2D	mNorm;
	float	mDist;

	//----------------------------------------------------------------//
	void	Flip			();
	void	Init			( const USVec2D& p1, const USVec2D& p2 );	// From a line segment
			USPlane2D		() {};
			~USPlane2D		() {};
};

//================================================================//
// USPlane3D
//================================================================//
class USPlane3D {
	public:

	USVec3D	mNorm;
	float	mDist;

	//----------------------------------------------------------------//
	void	Flip			();
	void	Init			( const USVec3D& p, const USVec3D& n );							// From a point and a normal
	void	Init			( const USVec3D& p1, const USVec3D& p2, const USVec3D& p3 );	// From a triangle; N = |( p2 - p1 ) x ( p3 - p1 )|
			USPlane3D		() {};
			~USPlane3D		() {};
};

#endif
