// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLDISTANCE_H
#define	ZLDISTANCE_H

//----------------------------------------------------------------//
#include <zl-util/ZLPlane.h>

//================================================================//
// ZLDist
//================================================================//
namespace ZLDist {
	
	float	PointToPlane2D		( const ZLVec2D& p, const ZLPlane2D& plane );
	float	PointToPoint		( const ZLVec2D& p1, const ZLVec2D& p2 );
	float	PointToPointSqrd	( const ZLVec2D& p1, const ZLVec2D& p2 );
	
	float	SnapToPlane			( ZLVec3D& p, const ZLPlane3D& plane );
	float	SnapToPlane2D		( ZLVec2D& p, const ZLPlane2D& plane );
	
	float	VecToPlane			( const ZLVec3D& v, const ZLPlane3D& p );
	float	VecToVec			( const ZLVec3D& v1, const ZLVec3D& v2 );
	float	VecToVecSqrd		( const ZLVec3D& v1, const ZLVec3D& v2 );
	
} // namespace ZLDist

#endif
