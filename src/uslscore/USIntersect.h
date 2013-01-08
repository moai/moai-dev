// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USINTERSECT_H
#define	USINTERSECT_H

#include <uslscore/USPlane.h>
#include <uslscore/USBox.h>

class USPrism;
class USRhombus;

//================================================================//
// USSect
//================================================================//
namespace USSect {

	enum {
		SECT_HIT		= 0,
		SECT_PARALLEL,
		SECT_TANGENT,
		SECT_NONE,
	};
	//----------------------------------------------------------------//
	// Returns the time along the vec, not the distance
	s32		BoxToPlane			( const USBox& b, const USPlane3D& p ); // 1, front; 0, sect; -1, behind;
	s32		PrismToPlane		( const USPrism& prism, const USPlane3D& p );
	u32		RayToBox			( const USVec3D& loc, const USVec3D& vec, const USBox& b, float& t );
	u32		RayToBox			( const USVec3D& loc, const USVec3D& vec, const USBox& b, float& t0, float& t1 );
	u32		RayToRay			( const USVec2D& locA, const USVec2D& vecA, const USVec2D& locB, const USVec2D& vecB, float &uA, float& uB );
	u32		RayToRect			( const USVec2D& loc, const USVec2D& vec, const USRect& r, float &t );
	u32		RayToRect			( const USVec2D& loc, const USVec2D& vec, const USRect& r, float &t0, float& t1 );
	s32		RhombusToPlane		( const USRhombus& rhombus, const USPlane3D& p );
	u32		VecToCircle			( float& t0, float& t1, const USVec2D& loc, const USVec2D& vec, const USVec2D& circleLoc, float radius );
	u32		VecToPlane			( const USVec2D& loc, const USVec2D& vec, const USPlane2D& p, float& t );
	u32		VecToPlane			( const USVec3D& loc, const USVec3D& vec, const USPlane3D& p, float& t );
	u32		VecToPlane			( const USVec3D& loc, const USVec3D& vec, const USPlane3D& p, float& t, USVec3D& result );
	u32		VecToSphere			( float& t0, float& t1, const USVec3D& loc, const USVec3D& vec, const USVec3D& sphereLoc, float radius );
	u32		VecToUnitCircle		( float& t0, float& t1, const USVec2D& loc, const USVec2D& vec );
	u32		VecToUnitSphere		( float& t0, float& t1, const USVec3D& loc, const USVec3D& vec );
	u32		XAxisToPlane		( float y, const USPlane2D& p, float& t );
	u32		YAxisToPlane		( float x, const USPlane2D& p, float& t );
}

#endif
