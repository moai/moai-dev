// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorShape.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorShape
//================================================================//

const ZLVec3D MOAIVectorShape::sNormal = ZLVec3D ( 0.0f, 0.0f, 1.0f );

//----------------------------------------------------------------//
void MOAIVectorShape::AddFillContours ( TESStesselator* tess ) {
	UNUSED ( tess );
}

//----------------------------------------------------------------//
void MOAIVectorShape::AddStrokeContours ( TESStesselator* tess ) {

	assert ( tess );

	TESStesselator* outline = tessNewTess ( 0 );
	assert ( outline );
	
	this->AddFillContours ( outline );
	tessTesselate ( outline, ( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	
	TESStesselator* exterior = tessNewTess ( 0 );
	TESStesselator* interior = tessNewTess ( 0 );
	
	assert ( exterior );
	assert ( interior );
	
	float width = this->mStyle.GetStrokeWidth ();
	
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
	
	this->StrokeBoundaries ( exterior, outline, exteriorWidth, true, false );
	tessTesselate ( exterior, TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( stroke, exterior );
	
	this->StrokeBoundaries ( interior, outline, interiorWidth, false, false );
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
bool MOAIVectorShape::CanGroup () {

	return this->mCanGroup;
}

//----------------------------------------------------------------//
void MOAIVectorShape::CopyAndTransformVertices ( ZLVec2D* vertices, const ZLAffine2D& transform, const ZLVec2D* src, u32 total ) {

	for ( u32 i = 0; i < total; ++i ) {
		vertices [ i ] = src [ i ];
		transform.Transform ( vertices [ i ]);
	}
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
MOAIVectorShape::MOAIVectorShape () :
	mCanGroup ( false ) {
}

//----------------------------------------------------------------//
MOAIVectorShape::~MOAIVectorShape () {
}

//----------------------------------------------------------------//
bool MOAIVectorShape::SetVertices ( const ZLVec2D* vertices, u32 total, bool closed ) {
	UNUSED ( vertices );
	UNUSED ( total );
	UNUSED ( closed );
	return false;
}

//----------------------------------------------------------------//
void MOAIVectorShape::Stroke ( TESStesselator* tess, const ZLVec2D* verts, int nVerts, float width, bool forward, bool interior ) {

	MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nVerts );
	MOAIVectorUtil::ComputeLineJoins ( joins, verts, nVerts, false, forward, interior );
	
	bool exact = interior ? ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_EXTERIOR ) : ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_INTERIOR );
	
	int contourVerts = MOAIVectorUtil::StrokeLine ( this->mStyle, 0, joins, nVerts, width, exact );
	ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * contourVerts );
	
	MOAIVectorUtil::StrokeLine ( this->mStyle, contour, joins, nVerts, width, exact );
	
	tessAddContour ( tess, 2, contour, sizeof ( ZLVec2D ), contourVerts );
}

//----------------------------------------------------------------//
void MOAIVectorShape::StrokeBoundaries ( TESStesselator* tess, TESStesselator* outline, float width, bool forward, bool interior ) {

	const float* verts = tessGetVertices ( outline );
	const int* elems = tessGetElements ( outline );
	int nelems = tessGetElementCount ( outline );

	for ( int i = 0; i < nelems; ++i ) {

		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];

		this->Stroke ( tess, ( const ZLVec2D* )&verts [ b * 2 ], n, width, forward, interior );
	}
}

//----------------------------------------------------------------//
void MOAIVectorShape::Tessalate ( MOAIVectorDrawing& drawing ) {
	
	u32 fillExtraID = this->mStyle.GetFillExtraID ();
	u32 strokeExtraID = this->mStyle.GetStrokeExtraID ();
	
	if ( this->mStyle.GetFillStyle () == MOAIVectorStyle::FILL_SOLID ) {
	
		if ( this->mStyle.GetExtrude () > 0.0f ) {
			
			TESStesselator* skirt = tessNewTess ( 0 );
			
			this->AddFillContours ( skirt );
			tessTesselate ( skirt, ( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, NVP, 2, ( const TESSreal* )&sNormal );
			
			drawing.WriteSkirt ( skirt, this->mStyle, this->mStyle.GetFillColor (), fillExtraID );
			
			tessDeleteTess ( skirt );
		}
		
		TESStesselator* triangles = tessNewTess ( 0 );
		
		this->AddFillContours ( triangles );
		tessTesselate ( triangles, ( int )this->mStyle.mWindingRule, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&sNormal );
		
		drawing.WriteTriangleIndices ( triangles, drawing.CountVertices ());
		drawing.WriteVertices ( triangles, this->mStyle.GetExtrude (), this->mStyle.mFillColor.PackRGBA (), fillExtraID );
		
		tessDeleteTess ( triangles );
	}
	
	if (( this->mStyle.GetStrokeStyle () != MOAIVectorStyle::STROKE_NONE ) && ( this->mStyle.GetStrokeWidth () > 0.0f )) {
		
		if ( this->mStyle.GetExtrude () > 0.0f ) {
			
			TESStesselator* skirt = tessNewTess ( 0 );
			
			this->AddStrokeContours ( skirt );
			tessTesselate ( skirt, ( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, NVP, 2, ( const TESSreal* )&sNormal );
			
			drawing.WriteSkirt ( skirt, this->mStyle, this->mStyle.GetStrokeColor (), strokeExtraID );
			
			tessDeleteTess ( skirt );
		}
		
		TESStesselator* triangles = tessNewTess ( 0 );
		
		this->AddStrokeContours ( triangles );
		tessTesselate ( triangles, TESS_WINDING_NONZERO, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&sNormal );
		
		drawing.WriteTriangleIndices ( triangles, drawing.CountVertices ());
		drawing.WriteVertices ( triangles, this->mStyle.GetExtrude (), this->mStyle.mStrokeColor.PackRGBA (), strokeExtraID );
		
		tessDeleteTess ( triangles );
	}
}
