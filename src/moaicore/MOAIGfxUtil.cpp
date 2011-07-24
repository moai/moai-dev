// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxUtil.h>

//================================================================//
// MOAIGfxUtil
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxUtil::BeginDrawing () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	float width = ( float )gfxDevice.GetWidth ();
	float height = ( float )gfxDevice.GetHeight ();

	MOAIViewport viewport;
	viewport.Init ( 0.0f, 0.0f, width, height );
	viewport.SetScale ( width, -height );
	viewport.SetOffset ( -1.0f, 1.0f );
	
	USAffine2D camera;
	camera.Ident ();
	MOAIGfxUtil::BeginDrawing ( viewport, camera );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::BeginDrawing ( MOAIViewport& viewport, USAffine2D& camera ) {

	// set us up the viewport
	USRect rect = viewport.GetRect ();
	
	GLint x = ( GLint )rect.mXMin;
	GLint y = ( GLint )rect.mYMin;
	
	GLsizei w = ( GLsizei )( rect.Width () + 0.5f );
	GLsizei h = ( GLsizei )( rect.Height () + 0.5f );
	
	glViewport ( x, y, w, h );

	if ( MOAIGfxDevice::Get ().GetPipelineMode () == MOAIGfxDevice::GL_PIPELINE_FIXED ) {

		// load view/proj
		glMatrixMode ( GL_PROJECTION );
		glLoadIdentity ();
		
		USAffine2D mtx;
		viewport.GetViewProjMtx ( camera, mtx );
		MOAIGfxUtil::LoadMatrix ( mtx );
		
		// load ident
		glMatrixMode ( GL_MODELVIEW );
		glLoadIdentity ();
	}
	
	glDisable ( GL_CULL_FACE );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::ClearColorBuffer ( u32 color ) {

	USColorVec colorVec;
	colorVec.SetRGBA ( color );
	
	glClearColor ( colorVec.mR, colorVec.mG, colorVec.mB, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::GetModelToWorldMtx ( USAffine2D& modelToWorld ) {

	USMatrix3D mtx;
	glGetFloatv ( GL_MODELVIEW_MATRIX, mtx.m );
	modelToWorld.Init ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::GetModelToWndMtx ( USAffine2D& modelToWnd ) {

	MOAIGfxUtil::GetModelToWorldMtx ( modelToWnd );

	USAffine2D mtx;
	MOAIGfxUtil::GetWorldToWndMtx ( mtx );
	modelToWnd.Append ( mtx );
}

//----------------------------------------------------------------//
USColorVec MOAIGfxUtil::GetPenColor () {

	float p [ 4 ];
	glGetFloatv ( GL_CURRENT_COLOR, p );
	
	USColorVec color;
	color.Set ( p [ 0 ], p [ 1 ], p [ 2 ], p [ 3 ]);
	return color;
}

//----------------------------------------------------------------//
void MOAIGfxUtil::GetViewProjMtx ( USAffine2D& viewProj ) {

	USMatrix3D mtx;
	glGetFloatv ( GL_PROJECTION_MATRIX, mtx.m );
	viewProj.Init ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::GetViewQuad ( USQuad& quad ) {

	USAffine2D invMtx;
	MOAIGfxUtil::GetViewProjMtx ( invMtx );
	invMtx.Inverse ();
	
	quad.mV [ 0 ].Init ( -1.0f, 1.0f );
	quad.mV [ 1 ].Init ( 1.0f, 1.0f );
	quad.mV [ 2 ].Init ( 1.0f, -1.0f );
	quad.mV [ 3 ].Init ( -1.0f, -1.0f );
	
	quad.Transform ( invMtx );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::GetViewRect ( USRect& rect ) {

	int params [ 4 ];
	glGetIntegerv ( GL_VIEWPORT, params );
	
	rect.mXMin = ( float )params [ 0 ];
	rect.mYMin = ( float )params [ 1 ];
	
	rect.mXMax = rect.mXMin + ( float )params [ 2 ];
	rect.mYMax = rect.mYMin + ( float )params [ 3 ];
}

//----------------------------------------------------------------//
void MOAIGfxUtil::GetWndToModelMtx ( USAffine2D& wndToModel ) {

	MOAIGfxUtil::GetModelToWndMtx ( wndToModel );
	wndToModel.Inverse ();
}

//----------------------------------------------------------------//
void MOAIGfxUtil::GetWndToWorldMtx ( USAffine2D& wndToWorld ) {

	USAffine2D mtx;

	USRect rect;
	MOAIGfxUtil::GetViewRect ( rect );
	
	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Inv Wnd
	
	wndToWorld.Translate ( -hWidth - rect.mXMin, -hHeight - rect.mYMin );
		
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ));
	wndToWorld.Append ( mtx );
	
	// inv viewproj
	MOAIGfxUtil::GetViewProjMtx ( mtx );
	mtx.Inverse ();
	
	wndToWorld.Append ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::GetWorldToModelMtx ( USAffine2D& worldToModel ) {

	USMatrix3D mtx;
	glGetFloatv ( GL_MODELVIEW_MATRIX, mtx.m );
	worldToModel.Init ( mtx );
	worldToModel.Inverse ();
}

//----------------------------------------------------------------//
void MOAIGfxUtil::GetWorldToWndMtx ( USAffine2D& worldToWnd, float xScale, float yScale ) {

	USAffine2D mtx;

	USRect viewport;
	MOAIGfxUtil::GetViewRect ( viewport );
	
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
void MOAIGfxUtil::LoadMatrix ( const USMatrix3D& mtx ) {

	glLoadMatrixf ( mtx.m );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::LoadMatrix ( const USAffine2D& mtx ) {

	USMatrix3D mtx3D;
	mtx3D.Init ( mtx );
	glLoadMatrixf ( mtx3D.m );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::MultMatrix ( const USMatrix3D& mtx ) {

	glMultMatrixf ( mtx.m );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::MultMatrix ( const USAffine2D& mtx ) {

	USMatrix3D mtx3D;
	mtx3D.Init ( mtx );
	glMultMatrixf ( mtx3D.m );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetPen ( u32 penColor, u32 penSize ) {

	MOAIGfxUtil::SetPenColor ( penColor );
	MOAIGfxUtil::SetPenSize ( penSize );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetPenColor ( u32 penColor ) {

	USColorVec color;
	color.SetRGBA ( penColor );
	color.LoadGfxState ();
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetPenColor ( const USColorVec& color ) {

	color.LoadGfxState ();
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetPenSize ( u32 penSize ) {

	glLineWidth (( GLfloat )penSize );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetScissorRect () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	glScissor ( 0, 0, ( int )gfxDevice.GetWidth (), ( int )gfxDevice.GetHeight ());
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetScissorRect ( USRect& rect ) {

	glScissor (( int )rect.mXMin, ( int )rect.mYMin, ( int )rect.Width (), ( int )rect.Height ());
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetScreenSpace () {

	USAffine2D wndToWorld;
	MOAIGfxUtil::GetWndToWorldMtx ( wndToWorld );
	
	glMatrixMode ( GL_MODELVIEW );
	MOAIGfxUtil::LoadMatrix ( wndToWorld );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetScreenSpace ( MOAIViewport& viewport ) {

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
	
	USAffine2D wndToNorm;
	viewport.GetWndToNormMtx ( wndToNorm );
	
	glMatrixMode ( GL_PROJECTION );
	MOAIGfxUtil::LoadMatrix ( wndToNorm );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetUVMtx () {

	glMatrixMode ( GL_TEXTURE );
	glLoadIdentity ();
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetUVMtx ( const USAffine2D& mtx ) {

	glMatrixMode ( GL_TEXTURE );
	MOAIGfxUtil::LoadMatrix ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetViewProjMtx () {

	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetViewProjMtx ( const USAffine2D& mtx ) {

	glMatrixMode ( GL_PROJECTION );
	MOAIGfxUtil::LoadMatrix ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetViewProjMtx ( const USMatrix3D& mtx ) {

	glMatrixMode ( GL_PROJECTION );
	MOAIGfxUtil::LoadMatrix ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetWorldMtx () {

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetWorldMtx ( const USAffine2D& mtx ) {

	glMatrixMode ( GL_MODELVIEW );
	MOAIGfxUtil::LoadMatrix ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxUtil::SetWorldMtx ( const USMatrix3D& mtx ) {

	glMatrixMode ( GL_MODELVIEW );
	MOAIGfxUtil::LoadMatrix ( mtx );
}

