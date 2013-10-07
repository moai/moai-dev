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
bool MOAIVectorShape::GroupShapes ( MOAIVectorShape** shapes, u32 total ) {
	UNUSED ( shapes );
	UNUSED ( total );
	return false;
}

//----------------------------------------------------------------//
bool MOAIVectorShape::GroupVertices ( MOAIVectorDrawing& drawing, u32 total ) {
	UNUSED ( drawing );
	UNUSED ( total );
	return false;
}

//----------------------------------------------------------------//
MOAIVectorShape::MOAIVectorShape () {
}

//----------------------------------------------------------------//
MOAIVectorShape::~MOAIVectorShape () {
}

//----------------------------------------------------------------//
void MOAIVectorShape::StrokeContours ( TESStesselator* outline, TESStesselator* stroke, float offset ) {

	const float* verts = tessGetVertices ( outline );
	const int* elems = tessGetElements ( outline );
	int nelems = tessGetElementCount ( outline );

	for ( int i = 0; i < nelems; ++i ) {

		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];

		ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * n );
		const ZLVec2D* zlVerts = ( const ZLVec2D* )&verts [ b * 2 ];
		
		for ( int i = 0; i < n; ++i ) {
			
			ZLVec2D v0 = zlVerts [( i + ( n - 1 )) % n ];
			ZLVec2D v1 = zlVerts [ i ];
			ZLVec2D v2 = zlVerts [( i + 1 ) % n ];
			
			ZLVec2D e0 = v1;
			e0.Sub ( v0 );
			
			ZLVec2D e1 = v2;
			e1.Sub ( v1 );
			
			ZLVec2D n = e0;
			n.Add ( e1 );
			n.Rotate90Clockwise ();
			n.Norm ();
			
			ZLVec2D c = v1;
			c.Add ( n, offset );
			contour [ i ] = c;
		}
		
		tessAddContour ( stroke, 2, contour, sizeof ( ZLVec2D ), n );
	}
}

//----------------------------------------------------------------//
void MOAIVectorShape::Tessalate ( MOAIVectorDrawing& drawing ) {

	TESStesselator* outline = tessNewTess ( 0 );
	
	if ( this->mStyle.GetFillStyle () == MOAIVectorStyle::FILL_SOLID ) {
	
		TESStesselator* triangles = tessNewTess ( 0 );
		this->ToTriangles ( outline, triangles );	
		drawing.WriteTriangleIndices ( triangles, drawing.CountVertices ());
		drawing.WriteVertices ( triangles, this->mStyle.mFillColor.PackRGBA ());
		tessDeleteTess ( triangles );
	}
	
	if (( this->mStyle.GetLineStyle () == MOAIVectorStyle::LINE_STROKE ) && ( this->mStyle.GetLineWidth () > 0.0f )) {
		
		TESStesselator* triangles = tessNewTess ( 0 );
		this->ToStroke ( outline, triangles );	
		drawing.WriteTriangleIndices ( triangles, drawing.CountVertices ());
		drawing.WriteVertices ( triangles, this->mStyle.mLineColor.PackRGBA ());
		tessDeleteTess ( triangles );
	}
	
	tessDeleteTess ( outline );
}

//----------------------------------------------------------------//
void MOAIVectorShape::ToStroke ( TESStesselator* outline, TESStesselator* triangles ) {

	const int NVP = 3;
	ZLVec3D normal ( 0.0f, 0.0f, -1.0f );

	assert ( outline );
	assert ( triangles );
	
	this->ToOutline ( outline );
	tessTesselate ( outline, ( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&normal );
	
	TESStesselator* exterior = tessNewTess ( 0 );
	TESStesselator* interior = tessNewTess ( 0 );
	
	assert ( exterior );
	assert ( interior );
	
	float width = this->mStyle.GetLineWidth () * 0.5f;
	float offset = this->mStyle.GetLineOffset ();
	
	this->StrokeContours ( outline, exterior, offset + width );
	tessTesselate ( exterior, TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&normal );
	
	this->StrokeContours ( outline, interior, offset - width );
	tessTesselate ( interior, TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&normal );
	
	const float* verts = tessGetVertices ( exterior );
	const int* elems = tessGetElements ( exterior );
	int nelems = tessGetElementCount ( exterior );

	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		tessAddContour ( triangles, 2, &verts [ b * 2 ], sizeof ( float ) * 2, n );
	}
	
	verts = tessGetVertices ( interior );
	elems = tessGetElements ( interior );
	nelems = tessGetElementCount ( interior );

	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		tessAddContour ( triangles, 2, &verts [ b * 2 ], sizeof ( float ) * 2, n );
	}
	
	tessTesselate ( triangles, TESS_WINDING_ODD, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&normal );
	
	tessDeleteTess ( exterior );
	tessDeleteTess ( interior );
}

//----------------------------------------------------------------//
void MOAIVectorShape::ToTriangles ( TESStesselator* outline, TESStesselator* triangles ) {

	const int NVP = 3;
	ZLVec3D normal ( 0.0f, 0.0f, -1.0f );

	assert ( outline );
	assert ( triangles );
	
	this->ToOutline ( outline );
	tessTesselate ( outline, ( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&normal );

	const float* verts = tessGetVertices ( outline );
	const int* elems = tessGetElements ( outline );
	int nelems = tessGetElementCount ( outline );

	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		tessAddContour ( triangles, 2, &verts [ b * 2 ], sizeof ( float ) * 2, n );
	}
	tessTesselate ( triangles, ( int )this->mStyle.mWindingRule, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&normal );
}
