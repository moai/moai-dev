// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLPOLYGON2D_H
#define	ZLPOLYGON2D_H

#include <zl-util/ZLRect.h>
#include <zl-util/ZLAffine2D.h>

//================================================================//
// ZLMetaPolygon2D
//================================================================//
template < typename TYPE >
class ZLMetaPolygon2D {
public:

	static const u32 POLY_KNOWN_BIT			= 0x01;
	static const u32 POLY_CLOCKWISE_BIT		= 0x02;
	static const u32 POLY_CONCAVE_BIT		= 0x04;
	
	static const u32 POLY_UNKNOWN			= 0x00;
	static const u32 POLY_COMPLEX			= 0x08;
	static const u32 POLY_CORRUPT			= 0xffffffff;
	
	static const u32 POLY_ANTICLOCKWISE_CONVEX		= POLY_KNOWN_BIT;
	static const u32 POLY_ANTICLOCKWISE_CONCAVE		= POLY_KNOWN_BIT | POLY_CONCAVE_BIT;
	static const u32 POLY_CLOCKWISE_CONVEX			= POLY_KNOWN_BIT | POLY_CLOCKWISE_BIT;
	static const u32 POLY_CLOCKWISE_CONCAVE			= POLY_KNOWN_BIT | POLY_CONCAVE_BIT | POLY_CLOCKWISE_BIT;

private:
	
	u32										mInfo;
	ZLMetaRect < TYPE >						mBounds;
	ZLLeanArray < ZLMetaVec2D < TYPE > >	mVertices;
	
	//----------------------------------------------------------------//
	u32 Analyze () {
	
		u32 totalVerts = this->mVertices.Size ();
		if ( totalVerts < 3 ) return POLY_CORRUPT;
		
		u32 status = POLY_KNOWN_BIT;
		
		TYPE sign = 0.0;
		
		u32 countPositive = 0;
		u32 countNegative = 0;
		
		for ( u32 i = 0; i < totalVerts; i++ ) {

			u32 nextIndex = ( i + 1 ) % totalVerts;

			ZLMetaVec2D < TYPE >& p0 = this->mVertices [ i ];
			ZLMetaVec2D < TYPE >& p1 = this->mVertices [ nextIndex ];
			ZLMetaVec2D < TYPE >& p2 = this->mVertices [( i + 2 ) % totalVerts ];
			
			ZLMetaVec2D < TYPE > v0 = p1;
			v0.Sub ( p0 );
			
			ZLMetaVec2D < TYPE > v1 = p2;
			v1.Sub ( p1 );
			
			TYPE z = v0.Cross ( v1 );
			if ( z > 0.0 ) {
				countPositive++;
			}
			else if ( z < 0.0f ) {
				countNegative++;
			}
			
			// we're going to brute force this here for now
			// TODO: check number of edges and do something efficient (Shamos-Hoey, for example)
			if ( i >= 2 ) {
				
				for ( u32 j = nextIndex == 0 ? 1 : 0; j < ( i - 1 ); ++j ) {
				
					ZLMetaVec2D < TYPE >& q0 = this->mVertices [ j ];
					ZLMetaVec2D < TYPE >& q1 = this->mVertices [ j + 1 ];
					
					if ( ZLSect::LineToLine ( p0, p1, q0, q1 ) == ZLSect::SECT_HIT ) {
						return POLY_COMPLEX;
					}
				}
			}
		}
		
		if ( countPositive == countNegative ) {
		
			return POLY_CORRUPT;
		}
		else {
		
			if ( countPositive < countNegative ) {
				status |= POLY_CLOCKWISE_BIT;
			}
			
			// both not 0; we're concave
			if ( countPositive && countNegative ) {
				status |= POLY_CONCAVE_BIT;
			}
		}
		return status;
	}
	
public:
	
	GET_CONST ( ZLMetaRect < TYPE >&, Bounds, mBounds )
	GET_CONST ( u32, Info, mInfo )

	GET_CONST ( ZLMetaVec2D < TYPE >*, Vertices, this->mVertices.Data ())
	GET_CONST ( size_t, Size, this->mVertices.Size ())

	//----------------------------------------------------------------//
	u32 Bless () {

		u32 totalVerts = this->mVertices.Size ();
		
		if ( totalVerts < 3 ) {
			this->mInfo = POLY_CORRUPT;
			return this->mInfo;
		}
		
		this->mBounds.Init (this->mVertices [ 0 ]);
		
		for ( u32 i = 1; i < totalVerts; ++i ) {
			ZLMetaVec2D < TYPE >& point = this->mVertices [ i ];
			this->mBounds.Grow ( point );
		}
		
		this->mInfo = this->Analyze ();
		return this->mInfo;
	}
	
	//----------------------------------------------------------------//
	bool GetDistance ( const ZLMetaVec2D < TYPE >& point, float& d, ZLMetaVec2D < TYPE >& p ) const {
		UNUSED ( d );
	
		bool foundResult = false;
		
		size_t totalVerts = this->mVertices.Size ();
		
		for ( size_t i = 0; i < totalVerts; ++i ) {
		
			ZLMetaVec2D < TYPE >& e0 = this->mVertices [ i ];
			ZLMetaVec2D < TYPE >& e1 = this->mVertices [( i + 1 ) % totalVerts ];
		
			// get the edge vector
			ZLMetaVec2D < TYPE > n = ZLMetaVec2D < TYPE >::Sub ( e1, e0 );
			
			// distance of edges and point along edge
			float edgeDist0		= n.Dot ( e0 );
			float edgeDist1		= n.Dot ( e1 );
			float edgeDist		= n.Dot ( point );
			
			ZLMetaVec2D < TYPE > candidateP;
			
			// if point lies inside edge
			if (( edgeDist0 <= edgeDist ) && ( edgeDist <= edgeDist1 )) {
				// snap the point onto the edge
				
				// edge normal
				n.Rotate90Anticlockwise ();
				n.NormSafe ();
				
				// snap
				candidateP = ZLMetaVec2D < TYPE >::Add ( point, n, n.Dot ( e0 ) - n.Dot ( point ));
			}
			else {
				// snap point to closest end point
				candidateP = ( edgeDist < edgeDist0 ) ? e0 : e1;
			}
			
			float candidateD = candidateP.Dist ( point );
			
			if (( !foundResult ) || ( candidateD < d )) {
			
				d = candidateD;
				p = candidateP;
				
				foundResult = true;
			}
		}
		return foundResult;
	}
	
	//----------------------------------------------------------------//
	cc8* GetInfoString () const {
	
		return ZLMetaPolygon2D < TYPE >::GetInfoString ( this->mInfo );
	}
	
	//----------------------------------------------------------------//
	static cc8* GetInfoString ( u32 info ) {

		switch ( info ) {
			case POLY_UNKNOWN:						return "POLY_UNKNOWN";
			case POLY_COMPLEX:						return "POLY_COMPLEX";
			case POLY_ANTICLOCKWISE_CONVEX:			return "POLY_ANTICLOCKWISE_CONVEX";
			case POLY_ANTICLOCKWISE_CONCAVE:		return "POLY_ANTICLOCKWISE_CONCAVE";
			case POLY_CLOCKWISE_CONVEX:				return "POLY_CLOCKWISE_CONVEX";
			case POLY_CLOCKWISE_CONCAVE:			return "POLY_CLOCKWISE_CONCAVE";
			case POLY_CORRUPT:						return "POLY_CORRUPT";
		};
		return "";
	}
	
	//----------------------------------------------------------------//
	const ZLVec2D& GetVertex ( u32 idx ) const {
	
		size_t size = this->mVertices.Size ();
		assert ( size );
	
		return this->mVertices [ idx % size ];
	}
	
	//----------------------------------------------------------------//
	void InitAsRect ( const ZLMetaRect < TYPE >& rect ) {
	
		this->mVertices.Init ( 4 );
		
		this [ 0 ].mX = rect.mXMin;
		this [ 0 ].mY = rect.mYMin;
		
		this [ 1 ].mX = rect.mXMax;
		this [ 1 ].mY = rect.mYMin;
		
		this [ 2 ].mX = rect.mXMax;
		this [ 2 ].mY = rect.mYMax;
		
		this [ 3 ].mX = rect.mXMin;
		this [ 3 ].mY = rect.mYMax;
		
		this->mInfo = POLY_UNKNOWN;
	}

	//----------------------------------------------------------------//
	bool PointInside ( const ZLMetaVec2D < TYPE >& p ) const {

		bool inPoly = false;	// start off assuming it _isn't_ in the polygon

		if ( !this->mBounds.Contains ( p )) return false;

		TYPE x = p.mX;
		TYPE y = p.mY;
		
		u32 totalVerts = this->mVertices.Size ();
		for ( u32 i = 0; i < totalVerts; i++ ) {

			ZLMetaVec2D < TYPE >& p1 = this->mVertices [ i ];
			ZLMetaVec2D < TYPE >& p2 = this->mVertices [( i + 1 ) % totalVerts ];
			
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
	void ReserveVertices ( size_t total ) {
	
		this->mVertices.Init ( total );
		this->mInfo = POLY_UNKNOWN;
	}

	//----------------------------------------------------------------//
	void SetVert ( u32 id, const ZLMetaVec2D < TYPE >& v ) {

		this->mVertices [ id ] = v;
		this->mInfo = POLY_UNKNOWN;
	}
	
	//----------------------------------------------------------------//
	void SetVert ( u32 id, TYPE x, TYPE y ) {

		ZLMetaVec2D < TYPE >& vert = this->mVertices [ id ];

		vert.mX = x;
		vert.mY = y;
		
		this->mInfo = POLY_UNKNOWN;
	}
	
	//----------------------------------------------------------------//
	void SetVertices ( const ZLMetaVec2D < TYPE >* vertices, size_t total ) {
	
		this->ReserveVertices ( total );
		memcpy ( this->mVertices.Data (), vertices, sizeof ( ZLMetaVec2D < TYPE > ) * total );
	}
	
	//----------------------------------------------------------------//
	void Transform ( const ZLMetaAffine2D < TYPE >& matrix ) {
	
		u32 totalVerts = this->mVertices.Size ();
		for ( u32 i = 0; i < totalVerts; i++ ) {
			matrix.Transform ( this->mVertices [ i ]);
		}
		
		if ( this->mInfo != POLY_UNKNOWN ) {
		
			TYPE c0r0 = matrix.m [ ZLMetaAffine2D < TYPE >::C0_R0 ];
			TYPE c1r1 = matrix.m [ ZLMetaAffine2D < TYPE >::C1_R1 ];
		
			if ((( c0r0 < 0.0f ) || ( c1r1 > 0.0f )) || (( c0r0 > 0.0f ) || ( c1r1 < 0.0f ))) {
				this->mInfo ^= POLY_CLOCKWISE_BIT;
			}
		}
	}
	
	//----------------------------------------------------------------//
	ZLMetaPolygon2D () :
		mInfo ( POLY_UNKNOWN ) {
	}

	//----------------------------------------------------------------//
	~ZLMetaPolygon2D () {
	}
};

typedef ZLMetaPolygon2D < float > ZLPolygon2D;
typedef ZLMetaPolygon2D < double > ZLPolygon2D64;

#endif
