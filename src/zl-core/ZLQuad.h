// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLQUAD_H
#define	ZLQUAD_H

#include <zl-core/ZLAffine2D.h>
#include <zl-core/ZLAffine3D.h>
#include <zl-core/ZLMatrix3x3.h>
#include <zl-core/ZLPlane.h>

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
	bool			Contains			( ZLReal x, ZLReal y ) const;
	ZLRect			GetFrame			() const;
	ZLPlane2D		GetPlane			( u32 id );
	ZLVec2D			GetVert				( u32 id );
	void			Init				( const ZLRect& rect );
	void			Init				( ZLReal x0, ZLReal y0, ZLReal x1, ZLReal y1 );
	bool			Intersect			( const ZLQuad& quad, ZLRect& result ) const;
	bool			Intersect			( const ZLRect& rect, ZLRect& result ) const;
	bool			Overlap				( const ZLQuad& quad ) const;
	bool			Overlap				( const ZLRect& rect ) const;
	static bool		RemapCoord			( const ZLQuad& src, const ZLQuad& dest, u32 triangleID, ZLReal x, ZLReal y, ZLVec2D& result );
	void			ReverseWinding		();
	void			Scale				( ZLReal xScale, ZLReal yScale );
	void			Transform			( const ZLAffine2D& transform );
	void			Transform			( const ZLAffine3D& transform );
	void			Transform			( const ZLMatrix3x3& transform );
	void			Translate			( ZLReal xOff, ZLReal yOff );
					ZLQuad				();
					ZLQuad				( const ZLRect& rect );
					ZLQuad				( ZLReal x0, ZLReal y0, ZLReal x1, ZLReal y1 );
};

#endif
