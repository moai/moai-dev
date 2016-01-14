// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCURVE_H
#define	ZLCURVE_H

#include <zl-util/ZLVec2D.h>

//================================================================//
// ZLCurve1D
//================================================================//
class ZLCurve1D {
public:
	
	//----------------------------------------------------------------//
	virtual float		Evaluate		( float t ) const = 0;
						ZLCurve1D		() {};
	virtual				~ZLCurve1D		() {};
};

//================================================================//
// ZLCurve2D
//================================================================//
class ZLCurve2D {
public:

	//----------------------------------------------------------------//
	virtual ZLVec2D		Evaluate		( float t ) const = 0;
						ZLCurve2D		() {};
	virtual				~ZLCurve2D		() {};
};

//================================================================//
// ZLCubicCurve1D
//================================================================//
class ZLCubicCurve1D :
	public ZLCurve1D {
public:

	float		mA;
	float		mB;
	float		mC;
	float		mD;
	
	//----------------------------------------------------------------//
	void		Init				( float a, float b, float c, float d );
};

//================================================================//
// ZLCubicCurve2D
//================================================================//
class ZLCubicCurve2D :
	public ZLCurve2D {
public:

	ZLVec2D		mP0;
	ZLVec2D		mP1;
	ZLVec2D		mP2;
	ZLVec2D		mP3;
	
	//----------------------------------------------------------------//
	void		Init				( const ZLVec2D& p0, const ZLVec2D& p1, const ZLVec2D& p2, const ZLVec2D& p3 );
	
};

//================================================================//
// ZLQuadraticCurve2D
//================================================================//
class ZLQuadraticCurve2D :
	public ZLCurve2D {
public:

	ZLVec2D		mP0;
	ZLVec2D		mP1;
	ZLVec2D		mP2;
	
	//----------------------------------------------------------------//
	void		Init				( const ZLVec2D& p0, const ZLVec2D& p1, const ZLVec2D& p2 );
};

//================================================================//
// ZLCatmullRom1D
//================================================================//
class ZLCatmullRom1D :
	public ZLCubicCurve1D {
public:

	//----------------------------------------------------------------//
	float		Evaluate			( float t ) const;
};

//================================================================//
// ZLCatmullRom2D
//================================================================//
class ZLCatmullRom2D :
	public ZLCubicCurve2D {
public:

	//----------------------------------------------------------------//
	ZLVec2D		Evaluate			( float t ) const;
};

//================================================================//
// ZLQuadraticBezier2D
//================================================================//
class ZLQuadraticBezier2D :
	public ZLQuadraticCurve2D {
public:

	//----------------------------------------------------------------//
	ZLVec2D		Evaluate			( float t ) const;
};

#endif
