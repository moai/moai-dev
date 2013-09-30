// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorShape.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorShape
//================================================================//

//----------------------------------------------------------------//
MOAIVectorShape::MOAIVectorShape () :
	mFillStyle ( FILL_SOLID ),
	mLineStyle ( LINE_NONE ),
	mWindingRule (( u32 )TESS_WINDING_ODD ) {
	
	this->mFillColor.SetWhite ();
	this->mLineColor.SetWhite ();
}

//----------------------------------------------------------------//
MOAIVectorShape::~MOAIVectorShape () {
}

//----------------------------------------------------------------//
void MOAIVectorShape::Tessalate ( MOAIVectorDrawing& drawing ) {

	TESStesselator* outline = tessNewTess ( 0 );
	TESStesselator* triangles = tessNewTess ( 0 );
	
	this->ToTriangles ( outline, triangles );
	
	u32 base = drawing.CountVertices ();
	
	drawing.WriteTriangleIndices ( triangles, base );
	drawing.WriteVertices ( triangles, this->mFillColor.PackRGBA ());
	
	tessDeleteTess ( outline );
	tessDeleteTess ( triangles );
}

//----------------------------------------------------------------//
void MOAIVectorShape::ToTriangles ( TESStesselator* outline, TESStesselator* triangles ) {

	const int NVP = 3;
	ZLVec3D normal ( 0.0f, 0.0f, -1.0f );

	assert ( outline );
	assert ( triangles );
	
	this->ToOutline ( outline );
	tessTesselate ( outline, ( int )this->mWindingRule, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&normal );
	
	const float* verts = tessGetVertices ( outline );
	const int* elems = tessGetElements ( outline );
	const int nelems = tessGetElementCount ( outline );

	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		tessAddContour ( triangles, 2, &verts [ b * 2 ], sizeof ( float ) * 2, n );
	}
	tessTesselate ( triangles, ( int )this->mWindingRule, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&normal );
}
