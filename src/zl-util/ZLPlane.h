// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLPLANE_H
#define	ZLPLANE_H

#include <zl-util/ZLVec2D.h>

//================================================================//
// ZLPlane2D
//================================================================//
class ZLPlane2D {
	public:

	USVec2D	mNorm;
	float	mDist;

	//----------------------------------------------------------------//
	void	Flip			();
	void	Init			( const USVec2D& p1, const USVec2D& p2 );	// From a line segment
			ZLPlane2D		() {};
			~ZLPlane2D		() {};
};

//================================================================//
// USPlane3D
//================================================================//
class USPlane3D {
	public:

	ZLVec3D	mNorm;
	float	mDist;

	//----------------------------------------------------------------//
	void	Flip			();
	void	Init			( const ZLVec3D& p, const ZLVec3D& n );							// From a point and a normal
	void	Init			( const ZLVec3D& p1, const ZLVec3D& p2, const ZLVec3D& p3 );	// From a triangle; N = |( p2 - p1 ) x ( p3 - p1 )|
			USPlane3D		() {};
			~USPlane3D		() {};
};

#endif
