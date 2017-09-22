// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIOverlapResolver.h>

using namespace MOAICollisionConsts;

//================================================================//
// MOAIOverlapResolver
//================================================================//

//----------------------------------------------------------------//
void CalculateInterval ( const ZLVec2D* poly, size_t nPoly, const ZLVec2D& axis, float& min, float& max ) {
	
    min = max = axis.Dot ( poly [ 0 ]);
	
    for ( size_t i = 0; i < nPoly; ++i ) {

        float d = axis.Dot ( poly [ i ]);
        if ( d < min ) {
			min = d;
		}
        else if ( d > max ) {
			max = d;
		}
    } 
}

//----------------------------------------------------------------//
bool AxisSeparatePolygon ( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1, const ZLVec2D& axis, float& t ) {

    float min0, max0;
    float min1, max1; 

    CalculateInterval ( poly0, nPoly0, axis, min0, max0 );
    CalculateInterval ( poly1, nPoly1, axis, min1, max1 );

	if (( max1 <= min0 ) || ( max0 <= min1 )) return false; // no overlap

    float d0 = max1 - min0; // if overlapped, d0 < 0
    float d1 = min1 - max0; // if overlapped, d1 > 0

	t = ABS ( d0 ) < ABS ( d1 ) ? d0 : d1;
	return true;
}

//----------------------------------------------------------------//
bool AxisSeparatePolygons ( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1, ZLVec2D& outN, float& outT ) {

	bool first = true;
	float bestT = 0.0f;
	ZLVec2D bestN ( 0.0f, 0.0f );

	for ( size_t i = 0; i < nPoly0; i++ ) {
	
		// get the edge normal
		ZLVec2D e0 = poly0 [ i ];
        ZLVec2D e1 = poly0 [( i + 1 ) % nPoly0 ];
        ZLVec2D e = e1 - e0;
        ZLVec2D n = ZLVec2D ( -e.mY, e.mX );
		n.Norm ();
		
		float t;
		
		if ( !AxisSeparatePolygon ( poly0, nPoly0, poly1, nPoly1, n, t )) return false;
		
		if (( first ) || ( ABS ( t ) < ABS ( bestT ))) {
			bestT = t;
			bestN = n;
			first = false;
		}
	}
	
	outN = bestN;
	outT = bestT;
	
	return true;
}

//----------------------------------------------------------------//
bool MOAIOverlapResolver::Separate ( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1 ) {

	float t0, t1;
	ZLVec2D n0, n1;

	if ( !AxisSeparatePolygons ( poly0, nPoly0, poly1, nPoly1, n0, t0 )) return false;
	if ( !AxisSeparatePolygons ( poly1, nPoly1, poly0, nPoly0, n1, t1 )) return false;
	
	if ( ABS ( t0 ) < ABS ( t1 )) {
		this->Accumulate ( n0.mX * t0, n0.mY * t0, 0.0f );
	}
	else {
		this->Accumulate ( n1.mX * -t1, n1.mY * -t1, 0.0f );
	}	
	return true;
}

