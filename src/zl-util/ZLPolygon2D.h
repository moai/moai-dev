// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLPOLYGON2D_H
#define	ZLPOLYGON2D_H

#include <zl-util/ZLRect.h>
#include <zl-util/ZLMatrix3x3.h>

//================================================================//
// ZLMetaPolygon2D
//================================================================//
template < typename TYPE >
class ZLMetaPolygon2D :
	public ZLLeanArray < ZLMetaVec2D < TYPE > > {
private:
	
	ZLMetaRect < TYPE >	mBounds;
	
public:

	GET ( const ZLMetaRect < TYPE >&, Bounds, mBounds )

	//----------------------------------------------------------------//
	void Bless () {

		u32 totalVerts = this->Size ();
		
		if ( totalVerts < 3 ) {
			return;
		}
		
		this->mBounds.Init (( *this )[ 0 ]);
		
		for ( u32 i = 1; i < totalVerts; ++i ) {
			ZLMetaVec2D < TYPE >& point = ( *this )[ i ];
			this->mBounds.Grow ( point );
		}
	}
	
	//----------------------------------------------------------------//
	void InitAsRect ( const ZLMetaRect < TYPE >& rect ) {
	
		this->Init ( 4 );
		
		this [ 0 ].mX = rect.mXMin;
		this [ 0 ].mY = rect.mYMin;
		
		this [ 1 ].mX = rect.mXMax;
		this [ 1 ].mY = rect.mYMin;
		
		this [ 2 ].mX = rect.mXMax;
		this [ 2 ].mY = rect.mYMax;
		
		this [ 3 ].mX = rect.mXMin;
		this [ 3 ].mY = rect.mYMax;
	}

	//----------------------------------------------------------------//
	bool PointInside ( const ZLMetaVec2D < TYPE >& p ) const {

		bool inPoly = false;	// start off assuming it _isn't_ in the polygon

		if ( !this->mBounds.Contains ( p )) return false;

		TYPE x = p.mX;
		TYPE y = p.mY;
		
		u32 totalVerts = this->Size ();
		for ( u32 i = 0; i < totalVerts; i++ ) {

			ZLMetaVec2D < TYPE >& p1 = ( *this )[ i ];
			ZLMetaVec2D < TYPE >& p2 = ( *this )[( i + 1 ) % totalVerts ];
			
			// Components of points
			TYPE p1X = p1.mX;
			TYPE p1Y = p1.mY;
			TYPE p2X = p2.mX;
			TYPE p2Y = p2.mY;
			
			// Reject line segs above, below or horizontal
			if ( y <= MIN ( p1Y, p2Y )) continue;
			if ( y > MAX ( p1Y, p2Y )) continue;
			if ( p1Y == p2Y ) continue;
			
			// Segment is behind point, so skip
			if ( x > MAX ( p1X, p2X )) continue;
			
			// x intersect w/ line seg
			TYPE xIntersect = (( y - p1Y ) * ( p2X - p1X ) / ( p2Y - p1Y )) + p1X;
			
			// If point is to the left of or on line, toggle state
			if ( p1X == p2X || x <= xIntersect ) {
				inPoly = !inPoly; // Crossed an edge, so flip the state
			}
		}
		return inPoly;
	}

	//----------------------------------------------------------------//
	void SetVert ( u32 id, const ZLMetaVec2D < TYPE >& v ) {

		( *this )[ id ] = v;
	}
	
	//----------------------------------------------------------------//
	void Transform ( const ZLMetaMatrix3x3 < TYPE >& matrix ) {
	
		u32 totalVerts = this->Size ();
		for ( u32 i = 0; i < totalVerts; i++ ) {
			matrix.Transform (( *this )[ i ]);
		}
	}
	
	//----------------------------------------------------------------//
	ZLMetaPolygon2D () {
	}

	//----------------------------------------------------------------//
	~ZLMetaPolygon2D () {
	}
};

typedef ZLMetaPolygon2D < float > ZLPolygon2D;
typedef ZLMetaPolygon2D < double > ZLPolygon2D64;

#endif
