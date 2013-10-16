// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorShape.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorShape
//================================================================//

const ZLVec3D MOAIVectorShape::sNormal = ZLVec3D ( 0.0f, 0.0f, 1.0f );

//----------------------------------------------------------------//
void MOAIVectorShape::AddFillContours ( MOAIVectorDrawing& drawing, TESStesselator* tess ) {
	UNUSED ( drawing );
	UNUSED ( tess );
}

//----------------------------------------------------------------//
void MOAIVectorShape::AddStrokeContours ( MOAIVectorDrawing& drawing, TESStesselator* tess ) {

	assert ( tess );

	TESStesselator* outline = tessNewTess ( 0 );
	assert ( outline );
	
	this->AddFillContours ( drawing, outline );
	tessTesselate ( outline, ( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	
	TESStesselator* exterior = tessNewTess ( 0 );
	TESStesselator* interior = tessNewTess ( 0 );
	
	assert ( exterior );
	assert ( interior );
	
	float width = this->mStyle.GetLineWidth ();
	
	float interiorWidth;
	float exteriorWidth;
	
	if ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_EXTERIOR ) {
		interiorWidth = 0.0f;
		exteriorWidth = width;
	}
	else if ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_INTERIOR ) {
		interiorWidth = width;
		exteriorWidth = 0.0f;
	}
	else {
		interiorWidth = width * 0.5f;
		exteriorWidth = interiorWidth;
	}
	
	TESStesselator* stroke = tessNewTess ( 0 );
	assert ( outline );
	
	this->StrokeBoundaries ( drawing, exterior, outline, exteriorWidth, true, false );
	tessTesselate ( exterior, TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( stroke, exterior );
	
	this->StrokeBoundaries ( drawing, interior, outline, interiorWidth, true, true );
	tessTesselate ( interior, TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( stroke, interior );
	
	tessTesselate ( stroke, TESS_WINDING_ODD, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( tess, stroke );
	
	tessDeleteTess ( interior );
	tessDeleteTess ( exterior );
	tessDeleteTess ( outline );
	tessDeleteTess ( stroke );
}

//----------------------------------------------------------------//
void MOAIVectorShape::CopyBoundaries ( TESStesselator* dest, TESStesselator* src ) {

	const float* verts = tessGetVertices ( src );
	const int* elems = tessGetElements ( src );
	int nelems = tessGetElementCount ( src );

	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		tessAddContour ( dest, 2, &verts [ b * 2 ], sizeof ( float ) * 2, n );
	}
}

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
void MOAIVectorShape::Stroke ( MOAIVectorDrawing& drawing, TESStesselator* tess, const ZLVec2D* verts, int nVerts, float width, bool forward, bool interior ) {

	MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nVerts );
	drawing.ComputeLineJoins ( joins, verts, nVerts, false, forward, interior );
	
	bool exact = interior ? ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_EXTERIOR ) : ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_INTERIOR );
	
	int contourVerts = drawing.StrokeLine ( 0, joins, nVerts, width, exact );
	ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * contourVerts );
	
	drawing.StrokeLine ( contour, joins, nVerts, width, exact );
	
	tessAddContour ( tess, 2, contour, sizeof ( ZLVec2D ), contourVerts );
}

//----------------------------------------------------------------//
void MOAIVectorShape::StrokeBoundaries ( MOAIVectorDrawing& drawing, TESStesselator* tess, TESStesselator* outline, float width, bool forward, bool interior ) {

	const float* verts = tessGetVertices ( outline );
	const int* elems = tessGetElements ( outline );
	int nelems = tessGetElementCount ( outline );

	for ( int i = 0; i < nelems; ++i ) {

		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];

		this->Stroke ( drawing, tess, ( const ZLVec2D* )&verts [ b * 2 ], n, width, forward, interior );
	}
}

//----------------------------------------------------------------//
void MOAIVectorShape::Tessalate ( MOAIVectorDrawing& drawing ) {
	
	if ( this->mStyle.GetFillStyle () == MOAIVectorStyle::FILL_SOLID ) {
	
		TESStesselator* triangles = tessNewTess ( 0 );
		
		this->AddFillContours ( drawing, triangles );
		tessTesselate ( triangles, ( int )this->mStyle.mWindingRule, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&sNormal );
			
		drawing.WriteTriangleIndices ( triangles, drawing.CountVertices ());
		drawing.WriteVertices ( triangles, this->mStyle.mFillColor.PackRGBA ());
		
		tessDeleteTess ( triangles );
	}
	
	if (( this->mStyle.GetLineStyle () == MOAIVectorStyle::LINE_STROKE ) && ( this->mStyle.GetLineWidth () > 0.0f )) {
		
		TESStesselator* triangles = tessNewTess ( 0 );
		
		this->AddStrokeContours ( drawing, triangles );
		tessTesselate ( triangles, TESS_WINDING_NONZERO, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&sNormal );
		
		drawing.WriteTriangleIndices ( triangles, drawing.CountVertices ());
		drawing.WriteVertices ( triangles, this->mStyle.mLineColor.PackRGBA ());
		
		tessDeleteTess ( triangles );
	}
}
