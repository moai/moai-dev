// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef POLYGON2D_H
#define	POLYGON2D_H

#include <uslscore/USMatrix3x3.h>

//================================================================//
// USMetaPolygon2D
//================================================================//
template < typename TYPE >
class USMetaPolygon2D {
private:
	
	USMetaRect < TYPE >	mBounds;
	USLeanArray < USMetaVec2D < TYPE > > mVerts;
	
	//----------------------------------------------------------------//
	
public:

	//----------------------------------------------------------------//
	void Bless () {

		u32 totalVerts = this->mVerts.Size ();
		
		if ( totalVerts < 3 ) {
			return;
		}
		
		this->mBounds.Init ( this->mVerts [ 0 ]);
		
		for ( u32 i = 1; i < totalVerts; ++i ) {
			this->mBounds.Grow ( this->mVerts [ i ]);
		}
	}

	//----------------------------------------------------------------//
	void Init ( u32 totalVerts ) {

		this->mVerts.Init ( totalVerts );
	}
	
	//----------------------------------------------------------------//
	void Init ( const USMetaRect < TYPE >& rect ) {
	
		this->Init ( 4 );
		
		this->mVerts [ 0 ].mX = rect.mXMin;
		this->mVerts [ 0 ].mY = rect.mYMin;
		
		this->mVerts [ 1 ].mX = rect.mXMax;
		this->mVerts [ 1 ].mY = rect.mYMin;
		
		this->mVerts [ 2 ].mX = rect.mXMax;
		this->mVerts [ 2 ].mY = rect.mYMax;
		
		this->mVerts [ 3 ].mX = rect.mXMin;
		this->mVerts [ 3 ].mY = rect.mYMax;
	}

	//----------------------------------------------------------------//
	bool PointInside ( const USMetaVec2D < TYPE >& p ) const {

		bool inPoly = false;	// start off assuming it _isn't_ in the polygon

		if ( !this->mBounds.Contains ( p )) return false;

		TYPE x = p.mX;
		TYPE y = p.mY;
		
		u32 totalVerts = this->mVerts.Size ();
		for ( u32 i = 0; i < totalVerts; i++ ) {

			USMetaVec2D < TYPE >& p1 = this->mVerts [ i ];
			USMetaVec2D < TYPE >& p2 = this->mVerts [( i + 1 ) % totalVerts ];
			
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
	void SetVert ( u32 id, const USMetaVec2D < TYPE >& v ) {

		this->mVerts [ id ] = v;
	}
	
	//----------------------------------------------------------------//
	void Transform ( const USMetaMatrix3x3 < TYPE >& matrix ) {
	
		u32 totalVerts = this->mVerts.Size ();
		for ( u32 i = 0; i < totalVerts; i++ ) {
			matrix.Transform ( this->mVerts [ i ]);
		}
	}
	
	//----------------------------------------------------------------//
	USMetaPolygon2D () {
	}

	//----------------------------------------------------------------//
	~USMetaPolygon2D () {
	}
};

typedef USMetaPolygon2D < float > USPolygon2D;
typedef USMetaPolygon2D < double > USPolygon2D64;

#endif
