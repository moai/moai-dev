//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"
#include <tesselator.h>

//----------------------------------------------------------------//
TEST_F ( GTESTMoaiContext, libtess2 ) {
	
	float contour0 [] = {
		40.000000, 36.770508,
		-33.541019, 0.000000,
		40.000000, -36.770508,
	};

	float contour1 [] = {
		-65.000000, 65.000000,
		-65.000000, -65.000000,
		65.000000, -65.000000,
		65.000000, -15.729490,
		10.000000, 11.770510,
		10.000000, -10.000000,
		-10.000000, -10.000000,
		-10.000000, 10.000000,
		10.000000, 10.000000,
		10.000006, -11.770506,
		65.000008, 15.729493,
		65.000000, 65.000000,
	};

	float contour2 [] = {
		115.000000, 74.270508,
		35.000004, 34.270512,
		35.000000, -34.270508,
		115.000000, -74.270508,
	};
	
	TESStesselator* tess = tessNewTess ( 0 );
	
	tessAddContour ( tess, 2, contour0, 8, 3 );
	tessAddContour ( tess, 2, contour1, 8, 12 );
	tessAddContour ( tess, 2, contour2, 8, 4 );
	
	float normal [] = { 0, 0, 1 };
	
	tessTesselate ( tess, TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0, normal );
	
	const float* verts = tessGetVertices ( tess );
	const int* elems = tessGetElements ( tess );
	int nelems = tessGetElementCount ( tess );

	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		
		ASSERT_TRUE ( n == 7 ); // should actually be 8; this is a floating point precision overflow error in libtess2!
		
		for ( int j = 0; j < n; ++j ) {
		
			size_t idx = ( b + j ) * 2;
			float x = verts [ idx ];
			float y = verts [ idx + 1 ];
		
			printf ( "%d %d: %g %g\n", i, j, x, y );
		}
	}
	
	tessDeleteTess ( tess );
}
