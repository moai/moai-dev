// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MATH_SECT_H
#define	MATH_SECT_H

#include <uslsext/USPlane.h>
#include <uslsext/USBox.h>

//----------------------------------------------------------------//
namespace USSect {

	enum {
		SECT_HIT,
		SECT_TANGENT,
		SECT_PARALLEL,
	};

	// Returns the time along the vec, not the distance
	s32		BoxToPlane			( USBox& b, USPlane3D& p ); // 1, front; 0, sect; -1, behind;
	s32		PrismToPlane		( USPrism& prism, USPlane3D& p );
	s32		RhombusToPlane		( USRhombus& rhombus, USPlane3D& p );
	u32		VecToCircle			( float& t0, float& t1, USVec2D& loc, USVec2D& vec, USVec2D& circleLoc, float radius );
	bool	VecToPlane			( USVec3D& loc, USVec3D& vec, USPlane3D& p, float& t );
	bool	VecToPlane			( USVec3D& loc, USVec3D& vec, USPlane3D& p, float& t, USVec3D& result );
	float	VecToPlane2D		( USVec2D& loc, USVec2D& vec, USPlane2D& p );
	float	VecToPlane2D		( USVec2D& loc, USVec2D& vec, USPlane2D& p, USVec2D& result );
	u32		VecToSphere			( float& t0, float& t1, USVec3D& loc, USVec3D& vec, USVec3D& sphereLoc, float radius );
	u32		VecToUnitCircle		( float& t0, float& t1, USVec2D& loc, USVec2D& vec );
	u32		VecToUnitSphere		( float& t0, float& t1, USVec3D& loc, USVec3D& vec );

} // namespace USSect

#endif
