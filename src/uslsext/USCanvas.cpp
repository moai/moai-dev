// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USBlendMode.h>
#include <uslsext/USColor.h>
#include <uslsext/USGfxDevice.h>
#include <uslsext/USGLLine.h>
#include <uslsext/USGLQuad.h>
#include <uslsext/USCanvas.h>
#include <uslsext/USMathConsts.h>
#include <uslsext/USTexture.h>

//================================================================//
// USCanvas
//================================================================//

//----------------------------------------------------------------//
void USCanvas::BeginDrawing () {

	USGfxDevice& gfxDevice = USGfxDevice::Get ();

	float width = ( float )gfxDevice.GetWidth ();
	float height = ( float )gfxDevice.GetHeight ();

	USViewport viewport;
	viewport.Init ( 0.0f, 0.0f, width, height );
	viewport.SetScale ( width, -height );
	viewport.SetOffset ( -1.0f, 1.0f );
	
	USAffine2D camera;
	camera.Ident ();
	USCanvas::BeginDrawing ( viewport, camera );
}

//----------------------------------------------------------------//
void USCanvas::BeginDrawing ( USViewport& viewport, USAffine2D& camera ) {

	// set us up the viewport
	USRect rect = viewport.GetRect ();
	
	GLint x = ( GLint )rect.mXMin;
	GLint y = ( GLint )rect.mYMin;
	
	GLsizei w = ( GLsizei )( rect.Width () + 0.5f );
	GLsizei h = ( GLsizei )( rect.Height () + 0.5f );
	
	glViewport ( x, y, w, h );

	// load view/proj
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	
	USAffine2D mtx;
	USCanvas::GetViewProjMtx ( viewport, camera, mtx );
	USCanvas::LoadMatrix ( mtx );
	
	// load ident
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
	
	glDisable ( GL_CULL_FACE );
}

//----------------------------------------------------------------//
void USCanvas::ClearColorBuffer ( u32 color ) {

	USColorVec colorVec;
	colorVec.SetRGBA ( color );
	
	glClearColor ( colorVec.mR, colorVec.mG, colorVec.mB, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
}

//----------------------------------------------------------------//
void USCanvas::DrawAxisGrid ( USVec2D loc, USVec2D vec, float size ) {

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
void USCanvas::DrawEllipseFill ( USRect& rect, u32 steps ) {

	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	USVec2D loc;
	loc.mX = rect.mXMin + xRad;
	loc.mY = rect.mYMin + yRad;

	USCanvas::DrawEllipseFill ( loc, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void USCanvas::DrawEllipseFill ( USVec2D& loc, float rad, u32 steps ) {

	USCanvas::DrawEllipseFill ( loc, rad, rad, steps );
}

//----------------------------------------------------------------//
void USCanvas::DrawEllipseFill ( USVec2D& loc, float xRad, float yRad, u32 steps ) {

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
void USCanvas::DrawEllipseFill ( float left, float top, float right, float bottom, u32 steps ) {

	USRect rect;
	rect.Init ( left, top, right, bottom );
	USCanvas::DrawEllipseFill ( rect, steps );
}

//----------------------------------------------------------------//
void USCanvas::DrawEllipseOutline ( USRect& rect, u32 steps ) {
	
	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	USVec2D loc;
	loc.mX = rect.mXMin + xRad;
	loc.mY = rect.mYMin + yRad;

	USCanvas::DrawEllipseOutline ( loc, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void USCanvas::DrawEllipseOutline ( USVec2D& loc, float rad, u32 steps ) {

	USCanvas::DrawEllipseOutline ( loc, rad, rad, steps );
}

//----------------------------------------------------------------//
void USCanvas::DrawEllipseOutline ( USVec2D& loc, float xRad, float yRad, u32 steps ) {

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
void USCanvas::DrawEllipseOutline ( float left, float top, float right, float bottom, u32 steps ) {

	USRect rect;
	rect.Init ( left, top, right, bottom );
	USCanvas::DrawEllipseOutline ( rect, steps );
}

//----------------------------------------------------------------//
void USCanvas::DrawGrid ( USRect& rect, u32 xCells, u32 yCells ) {

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

	USCanvas::DrawRectOutline ( rect );
}

//----------------------------------------------------------------//
void USCanvas::DrawLine ( USVec2D& v0, USVec2D& v1 ) {
	
	USGLLine glLine;
	glLine.SetVerts ( v0, v1 );
	glLine.Draw ();
}

//----------------------------------------------------------------//
void USCanvas::DrawLine ( float x0, float y0, float x1, float y1 ) {

	USGLLine glLine;
	glLine.SetVerts ( x0, y0, x1, y1 );
	glLine.Draw ();
}

//----------------------------------------------------------------//
void USCanvas::DrawPoint ( USVec2D& loc ) {

	USCanvas::DrawPoint ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
void USCanvas::DrawPoint ( float x, float y ) {

	float vtx [ 2 ];
	glVertexPointer ( 2, GL_FLOAT, 0, vtx );
	glEnableClientState ( GL_VERTEX_ARRAY );

	vtx [ 0 ] = x;
	vtx [ 1 ] = y;

	glDrawArrays ( GL_POINTS, 0, 1 );
}

//----------------------------------------------------------------//
////void USCanvas::DrawQuadFill ( USVec2D& v0, USVec2D& v1, USVec2D& v2, USVec2D& v3 ) {
////
////	USVec2D p0 = v0;
////	USVec2D p1 = v1;
////	USVec2D p2 = v2;
////	USVec2D p3 = v3;
////
////	USCanvas::BeginPrim ();
////	FXQuad fxQuad = USCanvas::NextQuad ();
////	
////	USCanvas::Project ( p0 );
////	USCanvas::Project ( p1 );
////	USCanvas::Project ( p2 );
////	USCanvas::Project ( p3 );
////
////	// USVec2D 0
////	fxQuad.mVtx [ 0 ].mX = p0.x;
////	fxQuad.mVtx [ 0 ].mY = p0.y;
////
////	// USVec2D 1
////	fxQuad.mVtx [ 1 ].mX = p1.x;
////	fxQuad.mVtx [ 1 ].mY = p1.y;
////	
////	// USVec2D 2
////	fxQuad.mVtx [ 2 ].mX = p2.x;
////	fxQuad.mVtx [ 2 ].mY = p2.y;
////	
////	// USVec2D 3
////	fxQuad.mVtx [ 3 ].mX = p3.x;
////	fxQuad.mVtx [ 3 ].mY = p3.y;
////	
////	USCanvas::EndPrim ();
////}

//----------------------------------------------------------------//
void USCanvas::DrawRay ( USVec2D loc, USVec2D vec ) {
	
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
void USCanvas::DrawRectEdges ( USRect rect, u32 edges ) {

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
void USCanvas::DrawRectFill ( USRect rect ) {

	rect.Bless ();
	USCanvas::DrawRectFill ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void USCanvas::DrawRectFill ( float left, float top, float right, float bottom ) {
	
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
void USCanvas::DrawRectOutline ( USRect& rect ) {
	
	USCanvas::DrawRectOutline ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void USCanvas::DrawRectOutline ( float left, float top, float right, float bottom ) {
	
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

//----------------------------------------------------------------//
void USCanvas::GetModelToWorldMtx ( USAffine2D& modelToWorld ) {

	USMatrix3D mtx;
	glGetFloatv ( GL_MODELVIEW_MATRIX, mtx.m );
	modelToWorld.Init ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::GetModelToWndMtx ( USAffine2D& modelToWnd ) {

	USCanvas::GetModelToWorldMtx ( modelToWnd );

	USAffine2D mtx;
	USCanvas::GetWorldToWndMtx ( mtx );
	modelToWnd.Append ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::GetNormToWndMtx ( const USViewport& viewport, USAffine2D& normToWnd ) {

	USAffine2D mtx;
	USRect rect = viewport.GetRect ();

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Wnd
	normToWnd.Scale ( hWidth, -hHeight );
	
	mtx.Translate ( hWidth + rect.mXMin, hHeight + rect.mYMin );
	normToWnd.Append ( mtx );
}

//----------------------------------------------------------------//
USColorVec USCanvas::GetPenColor () {

	float p [ 4 ];
	glGetFloatv ( GL_CURRENT_COLOR, p );
	
	USColorVec color;
	color.Set ( p [ 0 ], p [ 1 ], p [ 2 ], p [ 3 ]);
	return color;
}

//----------------------------------------------------------------//
void USCanvas::GetProjMtx ( const USViewport& viewport, USAffine2D& proj ) {
	
	USAffine2D mtx;
	USRect rect = viewport.GetRect ();
	
	// project
	float xScale = 2.0f / rect.Width ();
	float yScale = 2.0f / rect.Height ();
	
	proj.Scale ( xScale, yScale );
	
	mtx.Translate ( viewport.mOffset.mX, viewport.mOffset.mY );
	proj.Append ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::GetViewProjMtx ( USAffine2D& viewProj ) {

	USMatrix3D mtx;
	glGetFloatv ( GL_PROJECTION_MATRIX, mtx.m );
	viewProj.Init ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::GetViewProjMtx ( const USViewport& viewport, const USAffine2D& camera, USAffine2D& viewProj ) {

	USAffine2D mtx;
	USRect rect = viewport.GetRect ();

	// View
	viewProj.Inverse ( camera );
	
	// Project
	
	// rotate
	mtx.Rotate ( -viewport.mRotation * ( float )D2R );
	viewProj.Append ( mtx );

	// project
	USVec2D viewScale = viewport.GetScale ();
	float xScale = ( 2.0f / rect.Width ()) * viewScale.mX;
	float yScale = ( 2.0f / rect.Height ()) * viewScale.mY;
	
	mtx.Scale ( xScale, yScale );
	viewProj.Append ( mtx );
	
	// offset
	mtx.Translate ( viewport.mOffset.mX, viewport.mOffset.mY );
	viewProj.Append ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::GetViewProjMtxInv ( const USViewport& viewport, const USAffine2D& camera, USAffine2D& viewProjInv ) {

	USAffine2D mtx;
	USRect rect = viewport.GetRect ();

	// Inv Project
	
	// offset
	viewProjInv.Translate ( -viewport.mOffset.mX, -viewport.mOffset.mY );
	
	// project
	USVec2D viewScale = viewport.GetScale ();
	float invXScale = 1.0f / (( 2.0f / rect.Width () * viewScale.mX ));
	float invYScale = 1.0f / (( 2.0f / rect.Height () * viewScale.mY ));
	
	mtx.Scale ( invXScale, invYScale );
	viewProjInv.Append ( mtx );
	
	// rotate
	mtx.Rotate ( viewport.mRotation * ( float )D2R );
	viewProjInv.Append ( mtx );
	
	// Inv View
	viewProjInv.Append ( camera );
}

//----------------------------------------------------------------//
void USCanvas::GetViewRect ( USRect& rect ) {

	int params [ 4 ];
	glGetIntegerv ( GL_VIEWPORT, params );
	
	rect.mXMin = ( float )params [ 0 ];
	rect.mYMin = ( float )params [ 1 ];
	
	rect.mXMax = rect.mXMin + ( float )params [ 2 ];
	rect.mYMax = rect.mYMin + ( float )params [ 3 ];
}

//----------------------------------------------------------------//
void USCanvas::GetWndToModelMtx ( USAffine2D& wndToModel ) {

	USCanvas::GetModelToWndMtx ( wndToModel );
	wndToModel.Inverse ();
}

//----------------------------------------------------------------//
void USCanvas::GetWndToNormMtx ( const USViewport& viewport, USAffine2D& wndToNorm ) {

	USAffine2D mtx;
	USRect rect = viewport.GetRect ();

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Inv Wnd
	wndToNorm.Translate ( -hWidth - rect.mXMin, -hHeight - rect.mYMin );
	
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ));
	wndToNorm.Append ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::GetWndToWorldMtx ( USAffine2D& wndToWorld ) {

	USAffine2D mtx;

	USRect rect;
	USCanvas::GetViewRect ( rect );
	
	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Inv Wnd
	
	wndToWorld.Translate ( -hWidth - rect.mXMin, -hHeight - rect.mYMin );
		
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ));
	wndToWorld.Append ( mtx );
	
	// inv viewproj
	USCanvas::GetViewProjMtx ( mtx );
	mtx.Inverse ();
	
	wndToWorld.Append ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::GetWndToWorldMtx ( const USViewport& viewport, const USAffine2D& camera, USAffine2D& wndToWorld ) {

	USAffine2D mtx;
	USRect rect = viewport.GetRect ();

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Inv Wnd
	wndToWorld.Translate ( -hWidth - rect.mXMin, -hHeight - rect.mYMin );
	
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ));
	wndToWorld.Append ( mtx );

	USCanvas::GetViewProjMtxInv ( viewport, camera, mtx );
	wndToWorld.Append ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::GetWorldToModelMtx ( USAffine2D& worldToModel ) {

	USMatrix3D mtx;
	glGetFloatv ( GL_MODELVIEW_MATRIX, mtx.m );
	worldToModel.Init ( mtx );
	worldToModel.Inverse ();
}

//----------------------------------------------------------------//
void USCanvas::GetWorldToWndMtx ( USAffine2D& worldToWnd, float xScale, float yScale ) {

	USAffine2D mtx;

	USRect viewport;
	USCanvas::GetViewRect ( viewport );
	
	float hWidth = viewport.Width () * 0.5f;
	float hHeight = viewport.Height () * 0.5f;

	// viewproj
	USMatrix3D viewProj;
	glGetFloatv ( GL_PROJECTION_MATRIX, viewProj.m );
	worldToWnd.Init ( viewProj );
	
	// wnd
	mtx.Scale ( hWidth * xScale, hHeight * yScale );
	worldToWnd.Append ( mtx );
		
	mtx.Translate ( hWidth + viewport.mXMin, hHeight + viewport.mYMin );
	worldToWnd.Append ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::GetWorldToWndMtx ( const USViewport& viewport, const USAffine2D& camera, USAffine2D& worldToWnd ) {

	USAffine2D mtx;
	USRect rect		= viewport.GetRect ();
	
	float hWidth	= viewport.Width () * 0.5f;
	float hHeight	= viewport.Height () * 0.5f;

	USCanvas::GetViewProjMtx ( viewport, camera, worldToWnd );

	// Wnd
	mtx.Scale ( hWidth, -hHeight );
	worldToWnd.Append ( mtx );
	
	mtx.Translate ( hWidth + rect.mXMin, hHeight + rect.mYMin );
	worldToWnd.Append ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::LoadMatrix ( const USMatrix3D& mtx ) {

	glLoadMatrixf ( mtx.m );
}

//----------------------------------------------------------------//
void USCanvas::LoadMatrix ( const USAffine2D& mtx ) {

	USMatrix3D mtx3D;
	mtx3D.Init ( mtx );
	glLoadMatrixf ( mtx3D.m );
}

//----------------------------------------------------------------//
void USCanvas::MultMatrix ( const USMatrix3D& mtx ) {

	glMultMatrixf ( mtx.m );
}

//----------------------------------------------------------------//
void USCanvas::MultMatrix ( const USAffine2D& mtx ) {

	USMatrix3D mtx3D;
	mtx3D.Init ( mtx );
	glMultMatrixf ( mtx3D.m );
}

//----------------------------------------------------------------//
void USCanvas::SetPen ( u32 penColor, u32 penSize ) {

	USCanvas::SetPenColor ( penColor );
	USCanvas::SetPenSize ( penSize );
}

//----------------------------------------------------------------//
void USCanvas::SetPenColor ( u32 penColor ) {

	USColorVec color;
	color.SetRGBA ( penColor );
	color.LoadGfxState ();
}

//----------------------------------------------------------------//
void USCanvas::SetPenColor ( const USColorVec& color ) {

	color.LoadGfxState ();
}

//----------------------------------------------------------------//
void USCanvas::SetPenSize ( u32 penSize ) {

	glLineWidth (( GLfloat )penSize );
}

//----------------------------------------------------------------//
void USCanvas::SetScissorRect () {

	USGfxDevice& device = USGfxDevice::Get ();
	glScissor ( 0, 0, ( int )device.GetWidth (), ( int )device.GetHeight ());
}

//----------------------------------------------------------------//
void USCanvas::SetScissorRect ( USRect& rect ) {

	glScissor (( int )rect.mXMin, ( int )rect.mYMin, ( int )rect.Width (), ( int )rect.Height ());
}

//----------------------------------------------------------------//
void USCanvas::SetScreenSpace () {

	USAffine2D wndToWorld;
	USCanvas::GetWndToWorldMtx ( wndToWorld );
	
	glMatrixMode ( GL_MODELVIEW );
	USCanvas::LoadMatrix ( wndToWorld );
}

//----------------------------------------------------------------//
void USCanvas::SetScreenSpace ( USViewport& viewport ) {

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
	
	USAffine2D wndToNorm;
	USCanvas::GetWndToNormMtx ( viewport, wndToNorm );
	
	glMatrixMode ( GL_PROJECTION );
	USCanvas::LoadMatrix ( wndToNorm );
}

//----------------------------------------------------------------//
void USCanvas::SetTexture () {

	glBindTexture ( GL_TEXTURE_2D, 0 );
}

//----------------------------------------------------------------//
void USCanvas::SetTexture ( USTexture& texture ) {

	texture.Bind ();
}

//----------------------------------------------------------------//
void USCanvas::SetUVMtx () {

	glMatrixMode ( GL_TEXTURE );
	glLoadIdentity ();
}

//----------------------------------------------------------------//
void USCanvas::SetUVMtx ( const USAffine2D& mtx ) {

	glMatrixMode ( GL_TEXTURE );
	USCanvas::LoadMatrix ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::SetViewProjMtx () {

	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
}

//----------------------------------------------------------------//
void USCanvas::SetViewProjMtx ( const USAffine2D& mtx ) {

	glMatrixMode ( GL_PROJECTION );
	USCanvas::LoadMatrix ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::SetViewProjMtx ( const USMatrix3D& mtx ) {

	glMatrixMode ( GL_PROJECTION );
	USCanvas::LoadMatrix ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::SetWorldMtx () {

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
}

//----------------------------------------------------------------//
void USCanvas::SetWorldMtx ( const USAffine2D& mtx ) {

	glMatrixMode ( GL_MODELVIEW );
	USCanvas::LoadMatrix ( mtx );
}

//----------------------------------------------------------------//
void USCanvas::SetWorldMtx ( const USMatrix3D& mtx ) {

	glMatrixMode ( GL_MODELVIEW );
	USCanvas::LoadMatrix ( mtx );
}

