// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorTesselator.h>
#include <moai-sim/MOAIVectorShape.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorShape
//================================================================//

//----------------------------------------------------------------//
int MOAIVectorShape::AddFillContours ( SafeTesselator* tess ) {
	UNUSED ( tess );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorShape::AddStrokeContours ( SafeTesselator* tess, bool inside, bool outside ) {

	int error = 0;
	
	assert ( tess );

	SafeTesselator outline;
	
	this->AddFillContours ( &outline );
	error = outline.Tesselate ( ( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, 0, 0 );
	if ( error ) return error;
	
	SafeTesselator exterior;
	SafeTesselator interior;
	
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
	
	SafeTesselator stroke;
	
	if ( outside ) {
		this->StrokeBoundaries ( &exterior, &outline, exteriorWidth, true, false );
		error = exterior.Tesselate ( TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0 );
		if ( error ) return error;
		this->CopyBoundaries ( &stroke, &exterior );
	}
	
	if ( inside ) {
		this->StrokeBoundaries ( &interior, &outline, interiorWidth, false, false );
		error = interior.Tesselate ( TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0 );
		if ( error ) return error;
		this->CopyBoundaries ( &stroke, &interior );
	}
	
	error = stroke.Tesselate ( TESS_WINDING_ODD, TESS_BOUNDARY_CONTOURS, 0, 0 );
	if ( error ) return error;
	this->CopyBoundaries ( tess, &stroke );
	
	return 0;
}

//----------------------------------------------------------------//
bool MOAIVectorShape::CanGroup () {

	return this->mCanGroup;
}

//----------------------------------------------------------------//
void MOAIVectorShape::CopyBoundaries ( SafeTesselator* dest, SafeTesselator* src ) {

	const float* verts = tessGetVertices ( src->mTess );
	const int* elems = tessGetElements ( src->mTess );
	int nelems = tessGetElementCount ( src->mTess );

	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		dest->AddContour ( 2, &verts [ b * 2 ], sizeof ( float ) * 2, n );
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
void MOAIVectorShape::Stroke ( SafeTesselator* tess, const ZLVec2D* verts, int nVerts, float width, bool forward, bool interior ) {

	MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nVerts );
	MOAIVectorUtil::ComputeLineJoins ( joins, verts, nVerts, false, forward, interior );
	
	bool exact = interior ? ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_EXTERIOR ) : ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_INTERIOR ); // TODO: exact = width == 0.0f;
	
	int contourVerts = MOAIVectorUtil::StrokeLine ( this->mStyle, 0, joins, nVerts, width, exact );
	ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * contourVerts );
	
	MOAIVectorUtil::StrokeLine ( this->mStyle, contour, joins, nVerts, width, exact );
	
	tessAddContour ( tess->mTess, 2, contour, sizeof ( ZLVec2D ), contourVerts );
}

//----------------------------------------------------------------//
void MOAIVectorShape::StrokeBoundaries ( SafeTesselator* tess, SafeTesselator* outline, float width, bool forward, bool interior ) {

	const float* verts = tessGetVertices ( outline->mTess );
	const int* elems = tessGetElements ( outline->mTess );
	int nelems = tessGetElementCount ( outline->mTess );

	for ( int i = 0; i < nelems; ++i ) {

		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];

		this->Stroke ( tess, ( const ZLVec2D* )&verts [ b * 2 ], n, width, forward, interior );
	}
}

//----------------------------------------------------------------//
int MOAIVectorShape::Tesselate ( MOAIVectorTesselator& drawing, SafeTesselator* tess ) {

	assert ( tess );
	
	int error = 0;
	
	if ( this->mStyle.GetFillStyle () == MOAIVectorStyle::FILL_SOLID ) {
		error = this->AddFillContours ( tess );
		if ( error ) return error;
	}
	
	if (( this->mStyle.GetStrokeStyle () != MOAIVectorStyle::STROKE_NONE ) && ( this->mStyle.GetStrokeWidth () > 0.0f )) {
		error = this->AddStrokeContours ( tess, true, true );
		if ( error ) return error;
	}
	
	return error;
}

//----------------------------------------------------------------//
int MOAIVectorShape::Tesselate ( MOAIVectorTesselator& drawing, ZLStream* vtxStream, ZLStream* idxStream, MOAIVertexFormat* format ) {
	
	assert ( vtxStream );
	
	u32 fillExtraID = this->mStyle.GetFillExtraID ();
	u32 strokeExtraID = this->mStyle.GetStrokeExtraID ();
	int error = 0;
	
	bool isExtruded		= this->mStyle.GetExtrude () > 0.0f;
	bool isStroked		= ( this->mStyle.GetStrokeStyle () != MOAIVectorStyle::STROKE_NONE ) && ( this->mStyle.GetStrokeWidth () > 0.0f );
	bool isFilled		= this->mStyle.GetFillStyle () == MOAIVectorStyle::FILL_SOLID;
	bool isClosed		= this->IsClosed ();
	
	// fill solid w/ stroke
	// fill solid w/o stroke
	// no fill, just stroke
	
	if ( isFilled ) {
	
		// only draw the skirt if we are *not* stroked
		if ( isExtruded && !isStroked ) {
			
			SafeTesselator skirt;
			
			error = this->AddFillContours ( &skirt );
			if ( error ) return error;
			
			error = skirt.Tesselate (( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, NVP, 2 );
			if ( error ) return error;
			
			drawing.WriteSkirt ( &skirt, vtxStream, idxStream, format, this->mStyle, this->mStyle.GetFillColor (), fillExtraID );
		}
		
		SafeTesselator triangles;
		
		error = this->AddFillContours ( &triangles );
		if ( error ) return error;
		
		error = triangles.Tesselate (( int )this->mStyle.mWindingRule, TESS_POLYGONS, NVP, 2 );
		if ( error ) return error;
		
		if ( vtxStream && idxStream ) {
			drawing.WriteTriangles ( &triangles, vtxStream, idxStream, format, this->mStyle, this->mStyle.GetExtrude (), this->mStyle.mFillColor.PackRGBA (), fillExtraID );
		}
	}
	
	if ( isStroked ) {
		
		if ( isExtruded ) {
			
			SafeTesselator skirt;
			
			// only add the interior skirt we are *not* filled
			error = this->AddStrokeContours ( &skirt, !isFilled, true );
			if ( error ) return error;
			
			error = skirt.Tesselate (( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, NVP, 2 );
			if ( error ) return error;
			
			drawing.WriteSkirt ( &skirt, vtxStream, idxStream, format, this->mStyle, this->mStyle.GetStrokeColor (), strokeExtraID );
		}
		
		SafeTesselator triangles;
		
		error = this->AddStrokeContours ( &triangles, true, true );
		if ( error ) return error;
		
		error = triangles.Tesselate ( TESS_WINDING_NONZERO, TESS_POLYGONS, NVP, 2 );
		if ( error ) return error;
		
		if ( vtxStream && idxStream ) {
			drawing.WriteTriangles ( &triangles, vtxStream, idxStream, format, this->mStyle, this->mStyle.GetExtrude (), this->mStyle.mStrokeColor.PackRGBA (), strokeExtraID );
		}
	}
	
	return error;
}
