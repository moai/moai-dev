// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MATH_DIST_H
#define	MATH_DIST_H

//----------------------------------------------------------------//
#include <uslsext/USPlane.h>

//================================================================//
// USDist
//================================================================//
namespace USDist {
	
	float	PointToPlane2D		( USVec2D& p, USPlane2D& plane );
	float	PointToPoint		( USVec2D& p1, USVec2D& p2 );
	float	PointToPointSqrd	( USVec2D& p1, USVec2D& p2 );
	
	float	SnapToPlane			( USVec3D& p, USPlane3D& plane );
	float	SnapToPlane2D		( USVec2D& p, USPlane2D& plane );
	
	float	VecToPlane		( USVec3D& v, USPlane3D& p );
	float	VecToVec		( USVec3D& v1, USVec3D& v2 );
	float	VecToVecSqrd	( USVec3D& v1, USVec3D& v2 );
	
} // namespace USDist

#endif
