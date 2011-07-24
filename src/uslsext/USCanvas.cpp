// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USBlendMode.h>
#include <uslsext/USColor.h>
#include <uslsext/USDrawBuffer.h>
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

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();

	float width = ( float )drawBuffer.GetWidth ();
	float height = ( float )drawBuffer.GetHeight ();

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

	if ( USDrawBuffer::Get ().GetPipelineMode () == USDrawBuffer::GL_PIPELINE_FIXED ) {

		// load view/proj
		glMatrixMode ( GL_PROJECTION );
		glLoadIdentity ();
		
		USAffine2D mtx;
		USCanvas::GetViewProjMtx ( viewport, camera, mtx );
		USCanvas::LoadMatrix ( mtx );
		
		// load ident
		glMatrixMode ( GL_MODELVIEW );
		glLoadIdentity ();
	}
	
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

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	glScissor ( 0, 0, ( int )drawBuffer.GetWidth (), ( int )drawBuffer.GetHeight ());
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

