// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLDistance.h>
#include <zl-util/ZLFloat.h>
#include <zl-util/ZLIntersect.h>
#include <zl-util/ZLMemStream.h>
#include <zl-util/ZLPolygon2D.h>
#include <zl-util/ZLRadixSort32.h>

typedef ZLRadixKey32 < ZLVec2D > ZLRadixVec2D;

//================================================================//
// ZLPolygon2D
//================================================================//
	
//----------------------------------------------------------------//
u32 ZLPolygon2D::Analyze () {

	size_t totalVerts = this->mVertices.Size ();
	if ( totalVerts < 3 ) return POLY_CORRUPT;
	
	u32 status = POLY_KNOWN_BIT;
	
	//float sign = 0.0;
	
	size_t countPositive = 0;
	size_t countNegative = 0;
	
	float area = 0.0;
	
	for ( size_t i = 0; i < totalVerts; i++ ) {

		size_t nextIndex = ( i + 1 ) % totalVerts;

		ZLVec2D& p0 = this->mVertices [ i ];
		ZLVec2D& p1 = this->mVertices [ nextIndex ];
		ZLVec2D& p2 = this->mVertices [( i + 2 ) % totalVerts ];
		
		area += p0.Cross ( p1 );
		
		ZLVec2D v0 = p1;
		v0.Sub ( p0 );
		
		ZLVec2D v1 = p2;
		v1.Sub ( p1 );
		
		float z = v0.Cross ( v1 );
		if ( z > 0.0 ) {
			countPositive++;
		}
		else if ( z < 0.0f ) {
			countNegative++;
		}
		else {
			continue;
		}
		
		// we're going to brute force this here for now
		// TODO: check number of edges and do something efficient (Shamos-Hoey, for example)
		if ( i >= 2 ) {
			
			for ( size_t j = nextIndex == 0 ? 1 : 0; j < ( i - 1 ); ++j ) {
			
				ZLVec2D& q0 = this->mVertices [ j ];
				ZLVec2D& q1 = this->mVertices [ j + 1 ];
				
				if ( ZLSect::LineToLine ( p0, p1, q0, q1 ) == ZLSect::SECT_HIT ) {
					return POLY_COMPLEX;
				}
			}
		}
	}
	
	this->mArea = ABS ( area / 2 );
	
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

//----------------------------------------------------------------//
size_t ZLPolygon2D::Bless () {

	size_t totalVerts = this->mVertices.Size ();
	
	if ( totalVerts < 3 ) {
		this->mInfo = POLY_CORRUPT;
		return this->mInfo;
	}
	
	this->mBounds.Init (this->mVertices [ 0 ]);
	
	for ( size_t i = 1; i < totalVerts; ++i ) {
		ZLVec2D& point = this->mVertices [ i ];
		this->mBounds.Grow ( point );
	}
	
	this->mInfo = this->Analyze ();
	return this->mInfo;
}

//----------------------------------------------------------------//
bool ZLPolygon2D::Check ( u32 check ) const {

	switch ( check ) {
	
		case HAS_AREA:					return ( this->mInfo & POLY_KNOWN_BIT ) && !( this->mInfo & ( POLY_COMPLEX_BIT | POLY_CORRUPT_BIT ));
		case HAS_NO_AREA:				return ( this->mInfo & POLY_KNOWN_BIT ) && ( this->mInfo & ( POLY_COMPLEX_BIT | POLY_CORRUPT_BIT ));
		
		case IS_COMPLEX:				return this->mInfo == POLY_COMPLEX;
		case IS_CORRUPT:				return this->mInfo == POLY_CORRUPT;
		case IS_KNOWN:					return this->mInfo != POLY_UNKNOWN;
		case IS_UNKNOWN:				return this->mInfo == POLY_UNKNOWN;
		
		case IS_CONVEX:					return (( this->mInfo & SHAPE_MASK ) == POLY_KNOWN_BIT );
		case IS_CONCAVE:				return (( this->mInfo & SHAPE_MASK ) == ( POLY_KNOWN_BIT | POLY_CONCAVE_BIT ));
		
		case IS_CLOCKWISE:				return (( this->mInfo & WINDING_MASK ) == ( POLY_KNOWN_BIT | POLY_CLOCKWISE_BIT ));
		case IS_ANTICLOCKWISE:			return (( this->mInfo & WINDING_MASK ) == POLY_KNOWN_BIT );
		
		case IS_CLOCKWISE_CONVEX:		return (( this->mInfo & WINDING_SHAPE_MASK ) == ( POLY_KNOWN_BIT | POLY_CLOCKWISE_BIT ));
		case IS_CLOCKWISE_CONCAVE:		return (( this->mInfo & WINDING_SHAPE_MASK ) == ( POLY_KNOWN_BIT | POLY_CLOCKWISE_BIT | POLY_CONCAVE_BIT ));
		
		case IS_ANTICLOCKWISE_CONVEX:	return (( this->mInfo & WINDING_SHAPE_MASK ) == ( POLY_KNOWN_BIT ));
		case IS_ANTICLOCKWISE_CONCAVE:	return (( this->mInfo & WINDING_SHAPE_MASK ) == ( POLY_KNOWN_BIT | POLY_CONCAVE_BIT ));
	}
	return false;
}

//----------------------------------------------------------------//
void ZLPolygon2D::Clear () {

	this->mVertices.Clear ();
	this->mInfo = POLY_UNKNOWN;
}

//----------------------------------------------------------------//
//ZLSizeResult ZLPolygon2D::ConcaveHull ( ZLStream& input, size_t nPoints, int maxEdges, int maxPasses, float minIndent ) {
//
//	size_t reset = input.GetCursor ();
//
//	this->ConvexHull ( input, nPoints );
//	
//	size_t inputSize = nPoints * sizeof ( ZLVec2D );
//	
//	ZL_DECLARE_SCOPED_BUFFER ( points, ZLVec2D, inputSize, 1024 )
//	
//	input.Seek ( reset, SEEK_SET );
//	input.ReadBytes ( points, inputSize );
//	
//	// passes
//	for ( size_t i = 0; i < ( size_t )maxPasses; ++i ) {
//	
//		// cull exterior and face points
//		size_t nInside = 0;
//		for ( size_t j = 0; j < nPoints; ++j ) {
//			
//			if ( this->PointInside ( points [ j ]) == POINT_INSIDE ) {
//				points [ nInside ] = points [ j ];
//				nInside++;
//			}
//		}
//		nPoints = nInside;
//	
//		size_t nVerts = this->mVertices.Size ();
//		for ( size_t i = 0; i < nVerts; i++ ) {
//
//			ZLVec2D& p1 = this->mVertices [ i ];
//			ZLVec2D& p2 = this->mVertices [( i + 1 ) % nVerts ];
//		}
//	}
//	
//	//nPoints = interior.GetLength () / sizeof ( ZLVec2D );
//	interior.Seek ( 0, SEEK_SET );
//}

//----------------------------------------------------------------//
ZLSizeResult ZLPolygon2D::ConvexHull ( ZLStream& input, size_t nPoints, u32 sort ) {

	this->Clear ();

	size_t size = nPoints * sizeof ( ZLVec2D );

	ZL_DECLARE_SCOPED_BUFFER ( points, ZLVec2D, size, 1024 )
	ZL_DECLARE_SCOPED_BUFFER ( hull, ZLVec2D, 2 * size, 1024 )
	if ( !hull ) ZL_RETURN_SIZE_RESULT ( 0, ZL_ALLOCATION_ERROR )

	ZLSizeResult readResult = input.ReadBytes ( points, size );
	ZL_RETURN_SIZE_RESULT_IF_NOT_EQUAL ( readResult, size, 0, CODE )

	ZLSizeResult chainResult = ZLPolygon2D::MonotoneChain ( hull, points, nPoints, sort );
	
	if ( chainResult.mCode == ZL_OK ) {
	
		this->Clear ();
		ZL_HANDLE_ERROR_CODE ( this->ReserveVertices ( chainResult ), ZL_RETURN_SIZE_RESULT ( 0, ZL_ALLOCATION_ERROR ))
	
		memcpy ( this->mVertices, hull, chainResult * sizeof ( ZLVec2D ));
	
		this->Bless ();
	}
	return chainResult;
}

//----------------------------------------------------------------//
void ZLPolygon2D::Copy ( const ZLPolygon2D& src ) {

	this->Clear ();

	this->mInfo = src.mInfo;
	this->mBounds = src.mBounds;
	
	this->mVertices.Init ( src.mVertices.Size ());
	this->mVertices.CopyFrom ( src.mVertices );
}

//----------------------------------------------------------------//
float ZLPolygon2D::GetCorner ( size_t idx, ZLVec2D* normal ) {

	size_t size = this->mVertices.Size ();

	if ( size < 3 ) return 0.0f;
	
	idx = idx % size;

	ZLVec2D v0 = this->mVertices [ ( idx == 0 ? size : idx ) - 1 ];
	ZLVec2D v1 = this->mVertices [ idx ];
	ZLVec2D v2 = this->mVertices [ ( idx + 1 ) %  size ];
	
	ZLVec2D e0 = v1;
	e0.Sub ( v0 );
	e0.Norm ();
	
	ZLVec2D e1 = v2;
	e1.Sub ( v1 );
	e1.Norm ();
	
	ZLVec2D n = e0;
	n.Add ( e1 );
	n.Norm ();
	n.Rotate90Anticlockwise ();
	
	if ( normal ) {
		*normal = n;
	}
	
	return 2.0f * ACos ( n.Dot ( e0 ));
}

//----------------------------------------------------------------//
bool ZLPolygon2D::GetDistance ( const ZLVec2D& point, float& d ) const {

	ZLVec2D p;
	return this->GetDistance ( point, d, p );
}


//----------------------------------------------------------------//
bool ZLPolygon2D::GetDistance ( const ZLVec2D& point, float& d, ZLVec2D& p ) const {
	UNUSED ( d );

	bool foundResult = false;
	
	size_t totalVerts = this->mVertices.Size ();
	
	for ( size_t i = 0; i < totalVerts; ++i ) {
	
		ZLVec2D& e0 = this->mVertices [ i ];
		ZLVec2D& e1 = this->mVertices [( i + 1 ) % totalVerts ];
	
		// get the edge vector
		ZLVec2D n = ZLVec2D::Sub ( e1, e0 );
		
		// distance of edges and point along edge
		float edgeDist0		= n.Dot ( e0 );
		float edgeDist1		= n.Dot ( e1 );
		float edgeDist		= n.Dot ( point );
		
		ZLVec2D candidateP;
		
		// if point lies inside edge
		if (( edgeDist0 <= edgeDist ) && ( edgeDist <= edgeDist1 )) {
			// snap the point onto the edge
			
			// edge normal
			n.Rotate90Anticlockwise ();
			n.NormSafe ();
			
			// snap
			candidateP = ZLVec2D::Add ( point, n, n.Dot ( e0 ) - n.Dot ( point ));
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
cc8* ZLPolygon2D::GetInfoString () const {

	return ZLPolygon2D::GetInfoString ( this->mInfo );
}

//----------------------------------------------------------------//
cc8* ZLPolygon2D::GetInfoString ( u32 info ) {

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
const ZLVec2D& ZLPolygon2D::GetVertex ( size_t idx ) const {

	size_t size = this->mVertices.Size ();
	assert ( size );

	return this->mVertices [ idx % size ];
}

//----------------------------------------------------------------//
void ZLPolygon2D::InitAsRect ( const ZLRect& rect ) {

	this->mVertices.Init ( 4 );
	
	this->mVertices [ 0 ].mX = rect.mXMin;
	this->mVertices [ 0 ].mY = rect.mYMin;
	
	this->mVertices [ 1 ].mX = rect.mXMax;
	this->mVertices [ 1 ].mY = rect.mYMin;
	
	this->mVertices [ 2 ].mX = rect.mXMax;
	this->mVertices [ 2 ].mY = rect.mYMax;
	
	this->mVertices [ 3 ].mX = rect.mXMin;
	this->mVertices [ 3 ].mY = rect.mYMax;
	
	this->mInfo = POLY_UNKNOWN;
}

//----------------------------------------------------------------//
ZLSizeResult ZLPolygon2D::MonotoneChain ( ZLVec2D* hull, ZLVec2D* points, size_t nPoints, u32 sort ) {

	size_t nHull = 0;
	size_t size = nPoints * sizeof ( ZLVec2D );

	switch ( sort ) {

		case SORT_RADIX: {

			ZL_DECLARE_SCOPED_BUFFER ( radixBuffer, ZLRadixVec2D, 2 * size, 1024 )
			if ( !radixBuffer ) ZL_RETURN_SIZE_RESULT ( 0, ZL_ALLOCATION_ERROR );
			
			ZLRadixVec2D* swapBuffer = ( ZLRadixVec2D* )(( size_t )radixBuffer + size );

			for ( size_t i = 0; i < nPoints; ++i ) {
				const ZLVec2D& p = points [ i ];
				radixBuffer [ i ].mData = p;
				radixBuffer [ i ].mKey = ZLFloat::FloatToIntKey (( p.mX * ( float )nPoints ) + p.mY ); // sort by x then y
			}

			radixBuffer = RadixSort32 < ZLRadixVec2D >( radixBuffer, swapBuffer, ( u32 )nPoints );
			
			for ( size_t i = 0; i < nPoints; ++i ) {
				points [ i ] = radixBuffer [ i ].mData;
			}
			return ZLPolygon2D::MonotoneChain ( hull, points, nPoints, SORT_NONE );
		}
		
		case SORT_CSTDLIB: {
		
			qsort ( points, nPoints, sizeof ( ZLVec2D ), ZLPolygon2D::MonotoneChainComp );
			return ZLPolygon2D::MonotoneChain ( hull, points, nPoints, SORT_NONE );
		}
		
		case SORT_NONE:
		default: {
		
			for ( size_t i = 0; i < nPoints; i++ ) {
				while ( nHull >= 2 && hull [ nHull - 2 ].CrossJoint ( hull [ nHull - 1 ], points [ i ]) <= 0 ) nHull--;
				hull [ nHull++ ] = points [ i ];
			}

			for ( size_t i = nPoints - 2, t = nHull + 1; i != ( size_t )-1; i-- ) {
				while ( nHull >= t && hull [ nHull - 2 ].CrossJoint ( hull [ nHull - 1 ], points [ i ]) <= 0 ) nHull--;
				hull [ nHull++ ] = points [ i ];
			}
		}
	}

	ZL_RETURN_SIZE_RESULT ( nHull - 1, ZL_OK );
}

//----------------------------------------------------------------//
int ZLPolygon2D::MonotoneChainComp ( const void* p1, const void* p2 ) {

	const ZLVec2D* v0 = ( ZLVec2D* )p1;
	const ZLVec2D* v1 = ( ZLVec2D* )p2;
	
	if ( v0->mX < v1->mX ) return -1;
	if ( v0->mX > v1->mX ) return 1;
	
	if ( v0->mY < v1->mY ) return -1;
	if ( v0->mY > v1->mY ) return 1;
	
	return 0;
}

//----------------------------------------------------------------//
u32 ZLPolygon2D::PointInside ( const ZLVec2D& p, float pad ) const {

	if ( pad != 0.0f ) {
		
		float dist = 0.0f;
		
		if ( this->GetDistance ( p, dist )) {
			if ( dist <= ABS ( pad ) ) {
				return pad < 0.0f ? POINT_OUTSIDE : POINT_ON_EDGE;
			}
		}
	}

	bool inPoly = false; // start off assuming it _isn't_ in the polygon

	if ( this->mBounds.Contains ( p )) {

		float x = p.mX;
		float y = p.mY;
		
		size_t totalVerts = this->mVertices.Size ();
		for ( size_t i = 0; i < totalVerts; i++ ) {

			ZLVec2D& p1 = this->mVertices [ i ];
			ZLVec2D& p2 = this->mVertices [( i + 1 ) % totalVerts ];
			
			// Components of points
			float p1X = p1.mX;
			float p1Y = p1.mY;
			float p2X = p2.mX;
			float p2Y = p2.mY;
			
			// Segment is behind point, so skip
			if ( x > MAX ( p1X, p2X )) continue;
			
			// Special case if edge is parallel
			if ( p1Y == p2Y ) {
				if (( p1Y == y ) && (( x <= p1X ) || ( x <= p2X ))) return POINT_ON_EDGE;
				continue;
			}
			
			// Reject line segs above, below or horizontal
			if ( y <= MIN ( p1Y, p2Y )) continue;
			if ( y > MAX ( p1Y, p2Y )) continue;
			
			// x intersect w/ line seg
			float xIntersect = (( y - p1Y ) * ( p2X - p1X ) / ( p2Y - p1Y )) + p1X;
			
			// If we're on the line, return true
			if ( x == xIntersect ) return POINT_ON_EDGE;
			
			// If point is to the left of or on line, toggle state
			if ( p1X == p2X || x <= xIntersect ) {
				inPoly = !inPoly; // Crossed an edge, so flip the state
			}
		}
	}
	
	return inPoly ? POINT_INSIDE : POINT_OUTSIDE;
}

//----------------------------------------------------------------//
ZLResultCode ZLPolygon2D::ReserveVertices ( size_t total ) {

	this->mInfo = POLY_UNKNOWN;
	return this->mVertices.Init ( total );
}

//----------------------------------------------------------------//
void ZLPolygon2D::ReverseWinding () {

	size_t nVerts = this->mVertices.Size ();
	size_t nSwaps = nVerts >> 1;
	
	for ( size_t i = 0; i < nSwaps; ++i ) {
	
		size_t j = nVerts - i - 1;
		
		
		ZLVec2D& v0 = this->mVertices [ i ];
		ZLVec2D& v1 = this->mVertices [ j ];
		
		ZLVec2D swap = v0;
		
		v0 = v1;
		v1 = swap;
	}
	
	if ( this->mInfo & POLY_KNOWN_BIT ) {
		this->mInfo ^= POLY_CLOCKWISE_BIT;
	}
}

//----------------------------------------------------------------//
void ZLPolygon2D::SetVert ( size_t idx, const ZLVec2D& v ) {

	this->mVertices [ idx ] = v;
	this->mInfo = POLY_UNKNOWN;
}

//----------------------------------------------------------------//
void ZLPolygon2D::SetVert ( size_t idx, float x, float y ) {

	ZLVec2D& vert = this->mVertices [ idx ];

	vert.mX = x;
	vert.mY = y;
	
	this->mInfo = POLY_UNKNOWN;
}

//----------------------------------------------------------------//
void ZLPolygon2D::SetVertices ( const ZLVec2D* vertices, size_t total ) {

	this->ReserveVertices ( total );
	memcpy ( this->mVertices.Data (), vertices, sizeof ( ZLVec2D ) * total );
}

//----------------------------------------------------------------//
void ZLPolygon2D::Snap ( float xSnap, float ySnap ) {

	size_t totalVerts = this->mVertices.Size ();
	for ( size_t i = 0; i < totalVerts; i++ ) {
		
		ZLVec2D& vert = this->mVertices [ i ];
	
		vert.mX = xSnap == 0.0f ? vert.mX : floorf (( vert.mX / xSnap ) + 0.5f ) * xSnap;
		vert.mY = ySnap == 0.0f ? vert.mY : floorf (( vert.mY / ySnap ) + 0.5f ) * ySnap;
	}
}

//----------------------------------------------------------------//
void ZLPolygon2D::Transform ( const ZLAffine2D& matrix ) {

	size_t totalVerts = this->mVertices.Size ();
	for ( size_t i = 0; i < totalVerts; i++ ) {
		matrix.Transform ( this->mVertices [ i ]);
	}
	
	if ( this->mInfo != POLY_UNKNOWN ) {
	
		float c0r0 = matrix.m [ ZLAffine2D::C0_R0 ];
		float c1r1 = matrix.m [ ZLAffine2D::C1_R1 ];
	
		if ((( c0r0 < 0.0f ) || ( c1r1 > 0.0f )) || (( c0r0 > 0.0f ) || ( c1r1 < 0.0f ))) {
			this->mInfo ^= POLY_CLOCKWISE_BIT;
		}
	}
}

//----------------------------------------------------------------//
ZLPolygon2D::ZLPolygon2D () :
	mInfo ( POLY_UNKNOWN ),
	mArea ( 0 ) {
}

//----------------------------------------------------------------//
ZLPolygon2D::~ZLPolygon2D () {
}

