// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MATH_DIST_H
#define	MATH_DIST_H

//----------------------------------------------------------------//
#include <uslscore/USPlane.h>

//================================================================//
// USDist
//================================================================//
namespace USDist {
	
	float	PointToPlane2D		( const USVec2D& p, const USPlane2D& plane );
	float	PointToPoint		( const USVec2D& p1, const USVec2D& p2 );
	float	PointToPointSqrd	( const USVec2D& p1, const USVec2D& p2 );
	
	float	SnapToPlane			( USVec3D& p, const USPlane3D& plane );
	float	SnapToPlane2D		( USVec2D& p, const USPlane2D& plane );
	
	float	VecToPlane			( const USVec3D& v, const USPlane3D& p );
	float	VecToVec			( const USVec3D& v1, const USVec3D& v2 );
	float	VecToVecSqrd		( const USVec3D& v1, const USVec3D& v2 );
	
} // namespace USDist

#endif
