// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLPOLYGON2D_H
#define	ZLPOLYGON2D_H

#include <zl-core/ZLAccessors.h>
#include <zl-core/ZLAffine2D.h>
#include <zl-core/ZLLeanArray.h>
#include <zl-core/ZLRect.h>
#include <zl-core/ZLStream.h>

//================================================================//
// ZLPolygon2D
//================================================================//
class ZLPolygon2D {	
public:

	static const u32 POLY_KNOWN_BIT			= 0x01;
	static const u32 POLY_CLOCKWISE_BIT		= 0x02;
	static const u32 POLY_CONCAVE_BIT		= 0x04;
	static const u32 POLY_COMPLEX_BIT		= 0x08;
	static const u32 POLY_CORRUPT_BIT		= 0x10;
	
	static const u32 POLY_UNKNOWN					= 0;
	static const u32 POLY_COMPLEX					= POLY_KNOWN_BIT | POLY_COMPLEX_BIT;
	static const u32 POLY_CORRUPT					= POLY_KNOWN_BIT | POLY_CORRUPT_BIT;
	
	static const u32 POLY_ANTICLOCKWISE_CONVEX		= POLY_KNOWN_BIT;
	static const u32 POLY_ANTICLOCKWISE_CONCAVE		= POLY_KNOWN_BIT | POLY_CONCAVE_BIT;
	static const u32 POLY_CLOCKWISE_CONVEX			= POLY_KNOWN_BIT | POLY_CLOCKWISE_BIT;
	static const u32 POLY_CLOCKWISE_CONCAVE			= POLY_KNOWN_BIT | POLY_CONCAVE_BIT | POLY_CLOCKWISE_BIT;

	static const u32 WINDING_MASK			= POLY_KNOWN_BIT | POLY_CLOCKWISE_BIT;
	static const u32 SHAPE_MASK				= POLY_KNOWN_BIT | POLY_CONCAVE_BIT;
	static const u32 WINDING_SHAPE_MASK		= POLY_KNOWN_BIT | POLY_CLOCKWISE_BIT | POLY_CONCAVE_BIT;

	enum {
		POINT_OUTSIDE,
		POINT_INSIDE,
		POINT_ON_EDGE,
	};
	
	enum {
	
		HAS_AREA,
		HAS_NO_AREA,
	
		IS_COMPLEX,
		IS_CORRUPT,
		IS_KNOWN,
		IS_UNKNOWN,
		
		IS_CONVEX,
		IS_CONCAVE,
		
		IS_CLOCKWISE,
		IS_ANTICLOCKWISE,
		
		IS_CLOCKWISE_CONVEX,
		IS_CLOCKWISE_CONCAVE,
		
		IS_ANTICLOCKWISE_CONVEX,
		IS_ANTICLOCKWISE_CONCAVE,
	};

	typedef ZLReal VertexComponentType;
	static const size_t VERTEX_SIZE = 2 * sizeof ( VertexComponentType );

	enum {
		SORT_NONE,
		SORT_RADIX,
		SORT_CSTDLIB,
	};

private:
	
	u32										mInfo;
	ZLReal									mArea;
	ZLRect									mFrame;
	ZLLeanArray < ZLVec2D >					mVertices;
	
	//----------------------------------------------------------------//
	u32						Analyze					();
	static ZLSizeResult		MonotoneChain			( ZLVec2D* hull, ZLVec2D* points, size_t nPoints, u32 sort = SORT_CSTDLIB ); // hull buffer *must* be at least 2x the size of points
	static int				MonotoneChainComp		( const void* p1, const void* p2 );
	
public:
	
	GET_CONST ( ZLRect&, Frame, mFrame )
	GET_CONST ( u32, Info, mInfo )
	GET_CONST ( ZLReal, Area, mArea )

	GET_CONST ( ZLVec2D*, Vertices, this->mVertices.GetBuffer ())
	GET_CONST ( size_t, Size, this->mVertices.Size ())

	//----------------------------------------------------------------//
	size_t					Bless					();
	bool					Check					( u32 check ) const;
	void					Clear					();
	size_t					Clip					( const ZLPlane2D& plane, ZLStream& clippedPolyVerts, ZLStream& clippedPolySizes ) const;
	size_t					Clip					( const ZLPlane2D& plane, ZLStream& clippedPolyVerts, ZLStream& clippedPolySizes, void* buffer, size_t bufferSize ) const;
	size_t					Clip					( const ZLPolygon2D& poly, const ZLAffine3D* mtx, ZLStream& clippedPolyVerts, ZLStream& clippedPolySizes ) const;
	size_t					Clip					( const ZLPolygon2D& poly, const ZLAffine3D* mtx, ZLStream& clippedPolyVerts, ZLStream& clippedPolySizes, void* buffer, size_t bufferSize ) const;
	//ZLSizeResult			ConcaveHull				( ZLStream& input, size_t nPoints, int maxEdges, int maxPasses, ZLReal minIndent );
	ZLSizeResult			ConvexHull				( ZLStream& input, size_t nPoints, u32 sort = SORT_CSTDLIB );
	void					Copy					( const ZLPolygon2D& src );
	ZLReal					GetCorner				( ZLIndex idx, ZLVec2D* normal );
	bool					GetDistance				( const ZLVec2D& point, ZLReal& d ) const;
	bool					GetDistance				( const ZLVec2D& point, ZLReal& d, ZLVec2D& p ) const;
	cc8*					GetInfoString			() const;
	static cc8*				GetInfoString			( u32 info );
	const ZLVec2D&			GetVertex				( ZLIndex idx ) const;
	void					InitAsRect				( const ZLRect& rect );
	u32						PointInside				( const ZLVec2D& p, ZLReal pad = 0.0 ) const;
	ZLResultCode			ReserveVertices			( ZLSize total );
	void					ReverseWinding			();
	void					SetVert					( ZLIndex idx, const ZLVec2D& v );
	void					SetVert					( ZLIndex idx, ZLReal x, ZLReal y );
	void					SetVertices				( const ZLVec2D* vertices, ZLSize total );
	void					Snap					( ZLReal xSnap, ZLReal ySnap );
	void					Transform				( const ZLAffine2D& matrix );
							ZLPolygon2D				();
							~ZLPolygon2D			();
};

#endif
