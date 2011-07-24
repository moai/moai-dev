// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDraw.h>

//================================================================//
// MOAIDraw
//================================================================//

//----------------------------------------------------------------//
void MOAIDraw::DrawAxisGrid ( USVec2D loc, USVec2D vec, float size ) {

	USAffine2D mtx;
	USCanvas::GetViewProjMtx ( mtx );
	
	USAffine2D invMtx;
	invMtx.Inverse ( mtx );
	
	// Set the axis to the grid length so we can get the length back post-transform
	vec.SetLength ( size );
	
	mtx.Transform ( loc );
	mtx.TransformVec ( vec );

	// Get the axis unit vector
	USVec2D norm = vec;
	size = norm.NormSafe ();
	
	// Get the axis normal
	USVec2D perpNorm ( norm.mY, -norm.mX );
	
	// Project the corners of the viewport onto the axis to get the mix/max bounds
	float dot;
	float min;
	float max;
	
	USVec2D corner;
	
	// left, top
	corner.Init ( -1.0f, 1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = dot;
	max = dot;
	
	// right, top
	corner.Init ( 1.0f, 1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// right, bottom
	corner.Init ( 1.0f, -1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// left, bottom
	corner.Init ( -1.0f, -1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// Get the start andstop grids
	s32 start = ( s32 )( min / size ) - 1;
	s32 stop = ( s32 )( max / size ) + 1;
	
	// Set the pen to the first...
	USVec2D pen = norm;
	pen.Scale (( float )start * size );
	pen.Add ( loc );
	
	// Step along the axis to draw perpendicular grid lines
	USRect viewRect;
	viewRect.Init ( -1.0f, -1.0f, 1.0f, 1.0f );
	
	USGLLine glLine;
	
	for ( ; start < stop; ++start ) {
		
		USVec2D p0;
		USVec2D p1;
		
		if ( viewRect.GetIntersection ( pen, perpNorm, p0, p1 )) {
			
			invMtx.Transform ( p0 );
			invMtx.Transform ( p1 );
			
			glLine.SetVerts ( p0, p1 );
			glLine.Draw ();
		}
		
		pen.Add ( vec );
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseFill ( USRect& rect, u32 steps ) {

	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	USVec2D loc;
	loc.mX = rect.mXMin + xRad;
	loc.mY = rect.mYMin + yRad;

	MOAIDraw::DrawEllipseFill ( loc, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseFill ( USVec2D& loc, float rad, u32 steps ) {

	MOAIDraw::DrawEllipseFill ( loc, rad, rad, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseFill ( USVec2D& loc, float xRad, float yRad, u32 steps ) {

	static const u32 MAX = 64;
	if ( steps > MAX ) steps = MAX;
	float vtx [ MAX * 2 ];
	glVertexPointer ( 2, GL_FLOAT, 0, vtx );
	glEnableClientState ( GL_VERTEX_ARRAY );

	float angle = ( float )TWOPI / ( float )steps;
	float angleStep = ( float )PI;
	
	for ( u32 i = 0; i < steps; ++i, angleStep += angle ) {
	
		u32 vi = i << 1;
		vtx [ vi ] = loc.mX + ( Sin ( angleStep ) * xRad );
		vtx [ vi + 1 ] = loc.mY + ( Cos ( angleStep ) * yRad );
	}
	
	glDrawArrays ( GL_TRIANGLE_FAN, 0, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseFill ( float left, float top, float right, float bottom, u32 steps ) {

	USRect rect;
	rect.Init ( left, top, right, bottom );
	MOAIDraw::DrawEllipseFill ( rect, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseOutline ( USRect& rect, u32 steps ) {
	
	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	USVec2D loc;
	loc.mX = rect.mXMin + xRad;
	loc.mY = rect.mYMin + yRad;

	MOAIDraw::DrawEllipseOutline ( loc, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseOutline ( USVec2D& loc, float rad, u32 steps ) {

	MOAIDraw::DrawEllipseOutline ( loc, rad, rad, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseOutline ( USVec2D& loc, float xRad, float yRad, u32 steps ) {

	static const u32 MAX = 64;
	if ( steps > MAX ) steps = MAX;
	float vtx [ MAX * 2 ];
	glVertexPointer ( 2, GL_FLOAT, 0, vtx );
	glEnableClientState ( GL_VERTEX_ARRAY );

	float angle = ( float )TWOPI / ( float )steps;
	float angleStep = ( float )PI;
	
	for ( u32 i = 0; i < steps; ++i, angleStep += angle ) {
	
		u32 vi = i << 1;
		vtx [ vi ] = loc.mX + ( Sin ( angleStep ) * xRad );
		vtx [ vi + 1 ] = loc.mY + ( Cos ( angleStep ) * yRad );
	}
	
	glDrawArrays ( GL_LINE_LOOP, 0, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseOutline ( float left, float top, float right, float bottom, u32 steps ) {

	USRect rect;
	rect.Init ( left, top, right, bottom );
	MOAIDraw::DrawEllipseOutline ( rect, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawGrid ( USRect& rect, u32 xCells, u32 yCells ) {

	USGLLine glLine;

	if ( xCells > 1 ) {
		float xStep = rect.Width () / ( float )xCells;
		for ( u32 i = 1; i < xCells; ++i ) {
			float x = rect.mXMin + (( float )i * xStep );
			USVec2D v0 ( x, rect.mYMin );
			USVec2D v1 ( x, rect.mYMax );
			
			glLine.SetVerts ( v0, v1 );
			glLine.Draw ();
		}
	}

	if ( yCells > 1 ) {
		float yStep = rect.Height () / ( float )yCells;
		for ( u32 i = 1; i < yCells; ++i ) {
			float y = rect.mYMin + (( float )i * yStep );
			USVec2D v0 ( rect.mXMin, y );
			USVec2D v1 ( rect.mXMax, y );
			
			glLine.SetVerts ( v0, v1 );
			glLine.Draw ();
		}
	}

	MOAIDraw::DrawRectOutline ( rect );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawLine ( USVec2D& v0, USVec2D& v1 ) {
	
	USGLLine glLine;
	glLine.SetVerts ( v0, v1 );
	glLine.Draw ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawLine ( float x0, float y0, float x1, float y1 ) {

	USGLLine glLine;
	glLine.SetVerts ( x0, y0, x1, y1 );
	glLine.Draw ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawPoint ( USVec2D& loc ) {

	MOAIDraw::DrawPoint ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawPoint ( float x, float y ) {

	float vtx [ 2 ];
	glVertexPointer ( 2, GL_FLOAT, 0, vtx );
	glEnableClientState ( GL_VERTEX_ARRAY );

	vtx [ 0 ] = x;
	vtx [ 1 ] = y;

	glDrawArrays ( GL_POINTS, 0, 1 );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRay ( USVec2D loc, USVec2D vec ) {
	
	USAffine2D mtx;
	USCanvas::GetViewProjMtx ( mtx );
	
	USAffine2D invMtx;
	invMtx.Inverse ( mtx );
	
	mtx.Transform ( loc );
	mtx.TransformVec ( vec );
	
	USRect viewRect;
	viewRect.Init ( -1.0f, -1.0f, 1.0f, 1.0f );
	
	USVec2D p0;
	USVec2D p1;
	
	USGLLine glLine;
	
	if ( viewRect.GetIntersection ( loc, vec, p0, p1 )) {
		
		invMtx.Transform ( p0 );
		invMtx.Transform ( p1 );
		
		glLine.SetVerts ( p0, p1 );
		glLine.Draw ();
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectEdges ( USRect rect, u32 edges ) {

	rect.Bless ();

	USGLLine glLine;
	
	// right
	if ( edges & USRect::kRight ) {
		glLine.SetVerts ( rect.mXMax, rect.mYMin, rect.mXMax, rect.mYMax );
		glLine.Draw ();
	}

	// top
	if ( edges & USRect::kTop ) {			
		glLine.SetVerts ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMin );
		glLine.Draw ();
	}

	// left
	if ( edges & USRect::kLeft ) {			
		glLine.SetVerts ( rect.mXMin, rect.mYMin, rect.mXMin, rect.mYMax );
		glLine.Draw ();
	}

	// bottom
	if ( edges & USRect::kBottom ) {			
		glLine.SetVerts ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMax );
		glLine.Draw ();
	}	
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectFill ( USRect rect ) {

	rect.Bless ();
	MOAIDraw::DrawRectFill ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectFill ( float left, float top, float right, float bottom ) {
	
	glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState ( GL_COLOR_ARRAY );
	//glDisable ( GL_CULL_FACE );

	glEnable ( GL_BLEND );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	float vtx [ 8 ];
	glVertexPointer ( 2, GL_FLOAT, 0, vtx );
	glEnableClientState ( GL_VERTEX_ARRAY );
	
	vtx [ 0 ] = left;
	vtx [ 1 ] = top;
	
	vtx [ 2 ] = right;
	vtx [ 3 ] = top;
	
	vtx [ 4 ] = left;
	vtx [ 5 ] = bottom;
	
	vtx [ 6 ] = right;
	vtx [ 7 ] = bottom;
	
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectOutline ( USRect& rect ) {
	
	MOAIDraw::DrawRectOutline ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectOutline ( float left, float top, float right, float bottom ) {
	
	float vtx [ 8 ];
	glVertexPointer ( 2, GL_FLOAT, 0, vtx );
	glEnableClientState ( GL_VERTEX_ARRAY );
	
	vtx [ 0 ] = left;
	vtx [ 1 ] = top;
	
	vtx [ 2 ] = right;
	vtx [ 3 ] = top;
	
	vtx [ 4 ] = right;
	vtx [ 5 ] = bottom;
	
	vtx [ 6 ] = left;
	vtx [ 7 ] = bottom;
	
	glDrawArrays ( GL_LINE_LOOP, 0, 4 );
}
