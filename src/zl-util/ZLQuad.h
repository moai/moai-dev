// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLQUAD_H
#define	ZLQUAD_H

#include <zl-util/ZLAffine2D.h>
#include <zl-util/ZLAffine3D.h>
#include <zl-util/ZLMatrix3x3.h>
#include <zl-util/ZLPlane.h>

//================================================================//
// ZLQuad
//================================================================//
class ZLQuad {
private:

	//----------------------------------------------------------------//
	u32			Intersect			( const ZLQuad& quad, ZLVec2D* buffer, u32 max ) const;
	bool		IsSeparatingAxis	( const ZLVec2D& e0, const ZLVec2D& e1, const ZLVec2D& e2, const ZLVec2D& e3 ) const;

public:

	ZLVec2D mV [ 4 ];

	//----------------------------------------------------------------//
	bool			Contains			( float x, float y ) const;
	ZLRect			GetBounds			() const;
	ZLPlane2D		GetPlane			( u32 id );
	ZLVec2D			GetVert				( u32 id );
	void			Init				( const ZLRect& rect );
	void			Init				( float x0, float y0, float x1, float y1 );
	bool			Intersect			( const ZLQuad& quad, ZLRect& result ) const;
	bool			Intersect			( const ZLRect& rect, ZLRect& result ) const;
	bool			Overlap				( const ZLQuad& quad ) const;
	bool			Overlap				( const ZLRect& rect ) const;
	static bool		RemapCoord			( const ZLQuad& src, const ZLQuad& dest, u32 triangleID, float x, float y, ZLVec2D& result );
	void			ReverseWinding		();
	void			Scale				( float xScale, float yScale );
	void			Transform			( const ZLAffine2D& transform );
	void			Transform			( const ZLAffine3D& transform );
	void			Transform			( const ZLMatrix3x3& transform );
	void			Translate			( float xOff, float yOff );
};

#endif
