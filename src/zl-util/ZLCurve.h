// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCURVE_H
#define	ZLCURVE_H

#include <zl-util/ZLVec2D.h>

//================================================================//
// ZLCurve
//================================================================//
namespace ZLCurve {

	//----------------------------------------------------------------//
	float		CatmullRom1D	( float a, float b, float c, float d, float t );
	USVec2D		CatmullRom2D	( const USVec2D& p0, const USVec2D& p1, const USVec2D& p2, const USVec2D& p3, float t );
}

#endif
