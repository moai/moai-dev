// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorTesselator.h>
#include <moai-sim/MOAIVectorRect.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorRect
//================================================================//

//----------------------------------------------------------------//
int MOAIVectorRect::AddFillContours ( SafeTesselator& tess ) {

	ZLVec2D* verts = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * 4 );

	verts [ 0 ].mX = this->mRect.mXMin;
	verts [ 0 ].mY = this->mRect.mYMin;
	
	verts [ 1 ].mX = this->mRect.mXMax;
	verts [ 1 ].mY = this->mRect.mYMin;
	
	verts [ 2 ].mX = this->mRect.mXMax;
	verts [ 2 ].mY = this->mRect.mYMax;
	
	verts [ 3 ].mX = this->mRect.mXMin;
	verts [ 3 ].mY = this->mRect.mYMax;

	tess.AddContour ( 2, verts, sizeof ( ZLVec2D ), 4 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorRect::AddStrokeContours ( SafeTesselator& tess, bool inside, bool outside ) {

	return MOAIVectorShape::AddStrokeContours ( tess, inside, outside );
}

//----------------------------------------------------------------//
int MOAIVectorRect::CheckFastTrack ( MOAIVectorTesselator& drawing, u32 flags ) {
	UNUSED ( drawing );

	if (( this->mStyle.GetStrokeStyle () != MOAIVectorStyle::STROKE_NONE ) && ( this->mStyle.GetStrokeWidth () > 0.0f )) return FASTTRACK_FALLBACK;

	if ( !( flags & MOAIVectorTesselator::TESSELATE_FILLS )) return FASTTRACK_SKIP;
	
	if ( this->mStyle.GetFillStyle () != MOAIVectorStyle::FILL_SOLID ) return FASTTRACK_SKIP;

	ZLAffine2D& drawingToWorldMtx = this->mStyle.mDrawingToWorld;
	if (( drawingToWorldMtx.m [ ZLAffine2D::C0_R1 ] != 0.0f ) || ( drawingToWorldMtx.m [ ZLAffine2D::C1_R0 ] != 0.0f )) return FASTTRACK_FALLBACK;
	
	return FASTTRACK_OK;
}

//----------------------------------------------------------------//
void MOAIVectorRect::Init ( float xMin, float yMin, float xMax, float yMax ) {

	this->mRect.Init ( xMin, yMin, xMax, yMax );
}

//----------------------------------------------------------------//
bool MOAIVectorRect::IsClosed () {
	return true;
}

//----------------------------------------------------------------//
MOAIVectorRect::MOAIVectorRect () {
}

//----------------------------------------------------------------//
MOAIVectorRect::~MOAIVectorRect () {
}

//----------------------------------------------------------------//
void MOAIVectorRect::Read ( ZLStream& stream, MOAIVectorTesselatorWriter& writer ) {
	UNUSED ( writer );

	this->mRect = stream.Read < ZLRect >( this->mRect );
}

//----------------------------------------------------------------//
int MOAIVectorRect::Tesselate ( MOAIVectorTesselator& drawing, MOAIRegion& region, u32 flags ) {

	int fastTrack = this->CheckFastTrack ( drawing, flags );
	
	if ( fastTrack == FASTTRACK_SKIP ) return 0;
	if ( fastTrack == FASTTRACK_FALLBACK ) return MOAIVectorShape::Tesselate ( drawing, region, flags );
	
	//ZLAffine2D& drawingToWorldMtx = this->mStyle.mDrawingToWorld;
		
	ZLVec2D minVec ( this->mRect.mXMin, this->mRect.mYMin );
	ZLVec2D maxVec ( this->mRect.mXMax, this->mRect.mYMax );
	
	this->mStyle.mDrawingToWorld.Transform ( minVec );
	this->mStyle.mDrawingToWorld.Transform ( maxVec );

	region.ReservePolygons ( 1 );
	ZLPolygon2D& poly = region.GetPolygon ( 0 );
	
	poly.ReserveVertices ( 4 );
	poly.SetVert ( 0, ZLVec2D ( minVec.mX, minVec.mY ));
	poly.SetVert ( 1, ZLVec2D ( maxVec.mX, minVec.mY ));
	poly.SetVert ( 2, ZLVec2D ( maxVec.mX, maxVec.mY ));
	poly.SetVert ( 3, ZLVec2D ( minVec.mX, maxVec.mY ));

	region.Bless ();
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorRect::Tesselate ( MOAIVectorTesselator& drawing, ZLStream& vertexStream, ZLStream& indexStream, MOAIVertexFormat& format, u32 flags ) {

	int fastTrack = this->CheckFastTrack ( drawing, flags );
	
	if ( fastTrack == FASTTRACK_SKIP ) return 0;
	if ( fastTrack == FASTTRACK_FALLBACK ) return MOAIVectorShape::Tesselate ( drawing, vertexStream, indexStream, format, flags );

	ZLAffine2D& drawingToWorldMtx = this->mStyle.mDrawingToWorld;
		
	ZLVec2D minVec ( this->mRect.mXMin, this->mRect.mYMin );
	ZLVec2D maxVec ( this->mRect.mXMax, this->mRect.mYMax );
	
	this->mStyle.mDrawingToWorld.Transform ( minVec );
	this->mStyle.mDrawingToWorld.Transform ( maxVec );
	
	float xAxis = drawingToWorldMtx.m [ ZLAffine2D::C0_R0 ] < 0.0f ? -1.0f : 1.0f;
	float yAxis = drawingToWorldMtx.m [ ZLAffine2D::C1_R1 ] < 0.0f ? -1.0f : 1.0f;
	
	u32 fillColor = this->mStyle.mFillColor.PackRGBA ();
	float zt = this->mStyle.GetExtrude () + this->mStyle.GetZOffset ();
	
	u32 base			= drawing.CountVertices ( format, vertexStream );
	
	u32 fillExtraID		= this->mStyle.GetFillExtraID ();
	
	drawing.WriteVertex ( vertexStream, format, minVec.mX, minVec.mY, zt, 0.0f, 0.0f, 1.0f, fillColor, fillExtraID );
	drawing.WriteVertex ( vertexStream, format, maxVec.mX, minVec.mY, zt, 0.0f, 0.0f, 1.0f, fillColor, fillExtraID );
	drawing.WriteVertex ( vertexStream, format, maxVec.mX, maxVec.mY, zt, 0.0f, 0.0f, 1.0f, fillColor, fillExtraID );
	drawing.WriteVertex ( vertexStream, format, minVec.mX, maxVec.mY, zt, 0.0f, 0.0f, 1.0f, fillColor, fillExtraID );
	
	indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 1 ); indexStream.Write < u32 >( base + 2 );
	indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 2 ); indexStream.Write < u32 >( base + 3 );

	// only draw the skirt if we are *not* stroked
	if ( this->mStyle.GetExtrude () > 0.0f ) {
		
		float zb = this->mStyle.GetZOffset ();
		
		// bottom
		base = drawing.CountVertices ( format, vertexStream );
		
		drawing.WriteVertex ( vertexStream, format, minVec.mX, minVec.mY, zb, 0.0f, -yAxis, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, maxVec.mX, minVec.mY, zb, 0.0f, -yAxis, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, maxVec.mX, minVec.mY, zt, 0.0f, -yAxis, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, minVec.mX, minVec.mY, zt, 0.0f, -yAxis, 0.0f, fillColor, fillExtraID );
		
		indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 1 ); indexStream.Write < u32 >( base + 2 );
		indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 2 ); indexStream.Write < u32 >( base + 3 );
		
		// right
		base = drawing.CountVertices ( format, vertexStream );
		
		drawing.WriteVertex ( vertexStream, format, maxVec.mX, minVec.mY, zb, xAxis, 1.0f, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, maxVec.mX, maxVec.mY, zb, xAxis, 1.0f, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, maxVec.mX, maxVec.mY, zt, xAxis, 1.0f, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, maxVec.mX, minVec.mY, zt, xAxis, 1.0f, 0.0f, fillColor, fillExtraID );
		
		indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 1 ); indexStream.Write < u32 >( base + 2 );
		indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 2 ); indexStream.Write < u32 >( base + 3 );
		
		// top
		base = drawing.CountVertices ( format, vertexStream );
		
		drawing.WriteVertex ( vertexStream, format, maxVec.mX, maxVec.mY, zb, 0.0f, yAxis, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, minVec.mX, maxVec.mY, zb, 0.0f, yAxis, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, minVec.mX, maxVec.mY, zt, 0.0f, yAxis, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, maxVec.mX, maxVec.mY, zt, 0.0f, yAxis, 0.0f, fillColor, fillExtraID );
		
		indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 1 ); indexStream.Write < u32 >( base + 2 );
		indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 2 ); indexStream.Write < u32 >( base + 3 );
		
		// left
		base = drawing.CountVertices ( format, vertexStream );
		
		drawing.WriteVertex ( vertexStream, format, minVec.mX, maxVec.mY, zb, -xAxis, 1.0f, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, minVec.mX, minVec.mY, zb, -xAxis, 1.0f, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, minVec.mX, minVec.mY, zt, -xAxis, 1.0f, 0.0f, fillColor, fillExtraID );
		drawing.WriteVertex ( vertexStream, format, minVec.mX, maxVec.mY, zt, -xAxis, 1.0f, 0.0f, fillColor, fillExtraID );
		
		indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 1 ); indexStream.Write < u32 >( base + 2 );
		indexStream.Write < u32 >( base + 0 ); indexStream.Write < u32 >( base + 2 ); indexStream.Write < u32 >( base + 3 );
	}

	return 0;
}

//----------------------------------------------------------------//
void MOAIVectorRect::Write ( ZLStream& stream, MOAIVectorTesselatorWriter& writer ) const {
	UNUSED ( writer );

	stream.Write < ZLRect >( this->mRect );
}
