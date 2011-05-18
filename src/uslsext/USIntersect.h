// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MATH_SECT_H
#define	MATH_SECT_H

#include <uslsext/USPlane.h>
#include <uslsext/USBox.h>

//----------------------------------------------------------------//
namespace USSect {

	enum {
		SECT_HIT		= 0,
		SECT_PARALLEL,
		SECT_TANGENT,
	};
	//----------------------------------------------------------------//
	// Returns the time along the vec, not the distance
	s32		BoxToPlane			( USBox& b, USPlane3D& p ); // 1, front; 0, sect; -1, behind;
	s32		PrismToPlane		( USPrism& prism, USPlane3D& p );
	s32		RhombusToPlane		( USRhombus& rhombus, USPlane3D& p );
	u32		VecToCircle			( float& t0, float& t1, USVec2D& loc, USVec2D& vec, USVec2D& circleLoc, float radius );
	u32		VecToPlane			( USVec2D& loc, USVec2D& vec, USPlane2D& p, float& t );
	u32		VecToPlane			( USVec3D& loc, USVec3D& vec, USPlane3D& p, float& t );
	u32		VecToSphere			( float& t0, float& t1, USVec3D& loc, USVec3D& vec, USVec3D& sphereLoc, float radius );
	u32		VecToUnitCircle		( float& t0, float& t1, USVec2D& loc, USVec2D& vec );
	u32		VecToUnitSphere		( float& t0, float& t1, USVec3D& loc, USVec3D& vec );
	u32		XAxisToPlane		( float y, USPlane2D& p, float& t );
	u32		YAxisToPlane		( float x, USPlane2D& p, float& t );

} // namespace USSect

#endif
