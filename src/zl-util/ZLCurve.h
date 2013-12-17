// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCURVE_H
#define	ZLCURVE_H

#include <zl-util/ZLVec2D.h>

//================================================================//
// ZLAbstractVertexWriter2D
//================================================================//
// TODO: this should be moved someplace more general
class ZLAbstractVertexWriter2D {
public:

	//----------------------------------------------------------------//
	virtual void	WriteVertex					( const ZLVec2D& v ) = 0;
					ZLAbstractVertexWriter2D	() {};
	virtual			~ZLAbstractVertexWriter2D	() {};
};

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
// ZLCubicBezier2D
//================================================================//
class ZLCubicBezier2D :
	public ZLCubicCurve2D {
public:

	typedef void ( *AddVertFunc )( const ZLVec2D& vert, void* userdata );

private:

	//----------------------------------------------------------------//
	void				Bless					();
	void				FindInflectionDomain	( float t, float& t0, float& t1, float flatness = 0.25f ) const;
	u32					FindInflections			( float& t0, float& t1 ) const;
	void				FlattenProgressive		( ZLAbstractVertexWriter2D& writer, float flatness = 0.125f, float angle = 15.0f ) const;

public:

	enum {
		NONE,
		ONE_INFLECTION,
		TWO_INFLECTIONS,
		ONE_CUSP,
		DEGENERATE,
	};

	//----------------------------------------------------------------//
	float				Angle					();
	ZLVec2D				Evaluate				( float t ) const;
	void				Flatten					( ZLAbstractVertexWriter2D& writer, float flatness = 0.125f, float angle = 15.0f ) const;
	void				Split					( float t, ZLCubicBezier2D& left, ZLCubicBezier2D& right ) const;
	ZLCubicBezier2D		Split					( float t0, float t1 ) const;
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
