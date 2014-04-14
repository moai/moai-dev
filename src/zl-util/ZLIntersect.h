// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLINTERSECT_H
#define	ZLINTERSECT_H

#include <zl-util/ZLPlane.h>
#include <zl-util/ZLBox.h>

class ZLPrism;
class ZLRhombus;

//================================================================//
// ZLSect
//================================================================//
namespace ZLSect {

	enum {
		SECT_HIT		= 0,
		SECT_PARALLEL,
		SECT_TANGENT,
	};
	//----------------------------------------------------------------//
	// Returns the time along the vec, not the distance
	s32		BoxToPlane			( const ZLBox& b, const ZLPlane3D& p ); // 1, front; 0, sect; -1, behind;
	s32		PrismToPlane		( const ZLPrism& prism, const ZLPlane3D& p );
	s32		RayToBox			( const ZLBox& b, const ZLVec3D& loc, const ZLVec3D& dir, float &t );
	s32		RhombusToPlane		( const ZLRhombus& rhombus, const ZLPlane3D& p );
	u32		VecToCircle			( float& t0, float& t1, ZLVec2D& loc, ZLVec2D& vec, ZLVec2D& circleLoc, float radius );
	u32		VecToPlane			( const ZLVec2D& loc, const ZLVec2D& vec, const ZLPlane2D& p, float& t );
	u32		VecToPlane			( const ZLVec3D& loc, const ZLVec3D& vec, const ZLPlane3D& p, float& t );
	u32		VecToPlane			( const ZLVec3D& loc, const ZLVec3D& vec, const ZLPlane3D& p, ZLVec3D& result );
	u32		VecToPlane			( const ZLVec3D& loc, const ZLVec3D& vec, const ZLPlane3D& p, float& t, ZLVec3D& result );
	u32		VecToSphere			( float& t0, float& t1, const ZLVec3D& loc, const ZLVec3D& vec, const ZLVec3D& sphereLoc, float radius );
	u32		VecToUnitCircle		( float& t0, float& t1, const ZLVec2D& loc, const ZLVec2D& vec );
	u32		VecToUnitSphere		( float& t0, float& t1, const ZLVec3D& loc, const ZLVec3D& vec );
	u32		XAxisToPlane		( float y, const ZLPlane2D& p, float& t );
	u32		YAxisToPlane		( float x, const ZLPlane2D& p, float& t );
}

#endif
