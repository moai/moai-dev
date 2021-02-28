// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorCombo.h>
#include <moai-sim/MOAIVectorEllipse.h>
#include <moai-sim/MOAIVectorPoly.h>
#include <moai-sim/MOAIVectorRect.h>
#include <moai-sim/MOAIVectorTesselator.h>
#include <moai-sim/MOAIVectorShape.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorShape
//================================================================//

//----------------------------------------------------------------//
int MOAIVectorShape::AddFillContours ( SafeTesselator& tess ) {
	UNUSED ( tess );
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorShape::AddStrokeContours ( SafeTesselator& tess, bool inside, bool outside ) {

	int error = 0;

	SafeTesselator outline ( tess.GetContext ());
	
	this->AddFillContours ( outline );
	error = outline.Tesselate ( ( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, 0, 0 );
	if ( error ) return error;
	
	SafeTesselator exterior ( tess.GetContext ());
	SafeTesselator interior ( tess.GetContext ());
	
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
	
	SafeTesselator stroke ( tess.GetContext ());
	
	if ( outside ) {
		this->StrokeBoundaries ( exterior, outline, exteriorWidth, true, false );
		error = exterior.Tesselate ( TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0 );
		if ( error ) return error;
		stroke.CopyBoundaries ( exterior );
	}
	
	if ( inside ) {
		this->StrokeBoundaries ( interior, outline, interiorWidth, false, false );
		error = interior.Tesselate ( TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0 );
		if ( error ) return error;
		stroke.CopyBoundaries ( interior );
	}
	
	error = stroke.Tesselate ( TESS_WINDING_ODD, TESS_BOUNDARY_CONTOURS, 0, 0 );
	if ( error ) return error;
	tess.CopyBoundaries ( stroke );
	
	return 0;
}

//----------------------------------------------------------------//
bool MOAIVectorShape::CanGroup () {

	return this->mCanGroup;
}

//----------------------------------------------------------------//
MOAIVectorShape* MOAIVectorShape::Create ( u32 type ) {

	switch ( type ) {
		case COMBO:		return new MOAIVectorCombo ();
		case ELLIPSE:	return new MOAIVectorEllipse ();
		case POLY:		return new MOAIVectorPoly ();
		case RECT:		return new MOAIVectorRect ();
		case UNKNOWN:
		default:		return 0;
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
bool MOAIVectorShape::SetVertices ( const ZLVec2D* vertices, ZLSize total, bool closed ) {
	UNUSED ( vertices );
	UNUSED ( total );
	UNUSED ( closed );
	return false;
}

//----------------------------------------------------------------//
void MOAIVectorShape::Stroke ( SafeTesselator& tess, const ZLVec2D* verts, int nVerts, float width, bool forward, bool interior ) {

	MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nVerts );
	MOAIVectorUtil::ComputeLineJoins ( joins, verts, nVerts, false, forward, interior );
	
	bool exact = interior ? ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_EXTERIOR ) : ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_INTERIOR ); // TODO: exact = width == 0.0f;
	
	int contourVerts = MOAIVectorUtil::StrokeLine ( this->mStyle, 0, joins, nVerts, width, exact );
	ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * contourVerts );
	
	MOAIVectorUtil::StrokeLine ( this->mStyle, contour, joins, nVerts, width, exact );
	
	tess.AddContour2D ( contour, contourVerts, this->mStyle.mPrecision );
}

//----------------------------------------------------------------//
void MOAIVectorShape::StrokeBoundaries ( SafeTesselator& tess, SafeTesselator& outline, float width, bool forward, bool interior ) {

	const float* verts = tessGetVertices ( outline.mTess );
	const int* elems = tessGetElements ( outline.mTess );
	int nelems = tessGetElementCount ( outline.mTess );

	for ( int i = 0; i < nelems; ++i ) {

		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];

		this->Stroke ( tess, ( const ZLVec2D* )&verts [ b * 2 ], n, width, forward, interior );
	}
}

//----------------------------------------------------------------//
int MOAIVectorShape::Tesselate ( MOAIVectorTesselator& drawing, MOAIRegion& region, u32 flags ) {
	UNUSED ( drawing );
	UNUSED ( region );
	UNUSED ( flags );

	return 1;
}

//----------------------------------------------------------------//
int MOAIVectorShape::Tesselate ( MOAIVectorTesselator& drawing, SafeTesselator& tess, u32 flags ) {
	UNUSED ( drawing );

	int error = 0;
	
	if (( flags & MOAIVectorTesselator::TESSELATE_FILLS ) && ( this->mStyle.GetFillStyle () == MOAIVectorStyle::FILL_SOLID )) {
		error = this->AddFillContours ( tess );
		if ( error ) return error;
	}
	
	if (( flags & MOAIVectorTesselator::TESSELATE_STROKES ) && ( this->mStyle.GetStrokeStyle () != MOAIVectorStyle::STROKE_NONE ) && ( this->mStyle.GetStrokeWidth () > 0.0f )) {
		error = this->AddStrokeContours ( tess, true, true );
		if ( error ) return error;
	}
	
	return error;
}

//----------------------------------------------------------------//
int MOAIVectorShape::Tesselate ( MOAIVectorTesselator& drawing, ZLStream& vtxStream, ZLStream& idxStream, MOAIVertexFormat& format, u32 flags ) {
	
	ZLIndex fillExtraID		= this->mStyle.GetFillExtraID ();
	ZLIndex strokeExtraID	= this->mStyle.GetStrokeExtraID ();
	int error = 0;
	
	bool isExtruded		= this->mStyle.GetExtrude () > 0.0f;
	bool isStroked		= ( this->mStyle.GetStrokeStyle () != MOAIVectorStyle::STROKE_NONE ) && ( this->mStyle.GetStrokeWidth () > 0.0f );
	bool isFilled		= this->mStyle.GetFillStyle () == MOAIVectorStyle::FILL_SOLID;
	//bool isClosed		= this->IsClosed ();
	
	// fill solid w/ stroke
	// fill solid w/o stroke
	// no fill, just stroke
	
	if (( flags & MOAIVectorTesselator::TESSELATE_FILLS ) && isFilled ) {
	
		// only draw the skirt if we are *not* stroked
		if ( isExtruded && !isStroked ) {
			
			SafeTesselator skirt ( drawing.GetContext ());
			
			error = this->AddFillContours ( skirt );
			if ( error ) return error;
			
			error = skirt.Tesselate (( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, NVP, 2 );
			if ( error ) return error;
			
			drawing.WriteSkirt ( skirt, vtxStream, idxStream, format, this->mStyle, this->mStyle.GetFillColor (), fillExtraID );
		}
		
		SafeTesselator triangles ( drawing.GetContext ());
		
		error = this->AddFillContours ( triangles );
		if ( error ) return error;
		
		error = triangles.Tesselate (( int )this->mStyle.mWindingRule, TESS_POLYGONS, NVP, 2 );
		if ( error ) return error;
		
		drawing.WriteTriangles ( triangles, vtxStream, idxStream, format, this->mStyle, this->mStyle.GetExtrude () + this->mStyle.GetZOffset (), this->mStyle.mFillColor.PackRGBA (), fillExtraID );
	}
	
	if (( flags & MOAIVectorTesselator::TESSELATE_STROKES ) && isStroked ) {
		
		if ( isExtruded ) {
			
			SafeTesselator skirt ( drawing.GetContext ());
			
			// only add the interior skirt we are *not* filled
			error = this->AddStrokeContours ( skirt, !isFilled, true );
			if ( error ) return error;
			
			error = skirt.Tesselate (( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, NVP, 2 );
			if ( error ) return error;
			
			drawing.WriteSkirt ( skirt, vtxStream, idxStream, format, this->mStyle, this->mStyle.GetStrokeColor (), strokeExtraID );
		}
		
		SafeTesselator triangles ( drawing.GetContext ());
		
		error = this->AddStrokeContours ( triangles, true, true );
		if ( error ) return error;
		
		error = triangles.Tesselate ( TESS_WINDING_NONZERO, TESS_POLYGONS, NVP, 2 );
		if ( error ) return error;
		
		drawing.WriteTriangles ( triangles, vtxStream, idxStream, format, this->mStyle, this->mStyle.GetExtrude () + this->mStyle.GetZOffset (), this->mStyle.mStrokeColor.PackRGBA (), strokeExtraID );
	}
	
	return error;
}
