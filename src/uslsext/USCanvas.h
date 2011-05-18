// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	CANVAS_H
#define	CANVAS_H

#include <uslsext/USColor.h>
#include <uslsext/USMatrix3D.h>
#include <uslsext/USAffine2D.h>
#include <uslsext/USViewport.h>
#include <uslsext/USViewQuad.h>

class USTexture;

//================================================================//
// USCanvas
//================================================================//
class USCanvas {
public:

	//----------------------------------------------------------------//
	static void			BeginDrawing			();
	static void			BeginDrawing			( USViewport& viewport, USAffine2D& camera );
	
	static void			ClearColorBuffer		( u32 color );
	static void			DrawAxisGrid			( USVec2D loc, USVec2D vec, float size );
	static void			DrawEllipseFill			( USRect& rect, u32 steps );
	static void			DrawEllipseFill			( USVec2D& loc, float rad, u32 steps );
	static void			DrawEllipseFill			( USVec2D& loc, float xRad, float yRad, u32 steps );
	static void			DrawEllipseFill			( float left, float top, float right, float bottom , u32 steps);
	static void			DrawEllipseOutline		( USRect& rect, u32 steps );
	static void			DrawEllipseOutline		( USVec2D& loc, float rad, u32 steps );
	static void			DrawEllipseOutline		( USVec2D& loc, float xRad, float yRad, u32 steps );
	static void			DrawEllipseOutline		( float left, float top, float right, float bottom, u32 steps );
	static void			DrawGrid				( USRect& rect, u32 xCells, u32 yCells );
	static void			DrawLine				( USVec2D& v0, USVec2D& v1 );
	static void			DrawLine				( float x0, float y0, float x1, float y1 );
	static void			DrawPoint				( USVec2D& loc );
	static void			DrawPoint				( float x, float y );
	static void			DrawRay					( USVec2D loc, USVec2D vec );
	static void			DrawRectEdges			( USRect rect, u32 edges );
	static void			DrawRectFill			( USRect rect );
	static void			DrawRectFill			( float left, float top, float right, float bottom );
	static void			DrawRectOutline			( USRect& rect );
	static void			DrawRectOutline			( float left, float top, float right, float bottom );
	static void			GetModelToWndMtx		( USAffine2D& modelToWnd );
	static void			GetModelToWorldMtx		( USAffine2D& modelToWorld );
	static void			GetNormToWndMtx			( const USViewport& viewport, USAffine2D& normToWnd );
	static USColorVec	GetPenColor				();
	static void			GetProjMtx				( const USViewport& viewport, USAffine2D& proj );
	
	static void			GetViewProjMtx			( USAffine2D& viewProj );
	static void			GetViewProjMtx			( const USViewport& viewport, const USAffine2D& camera, USAffine2D& viewProj );
	static void			GetViewProjMtxInv		( const USViewport& viewport, const USAffine2D& camera, USAffine2D& viewProjInv );
	
	static void			GetViewRect				( USRect& rect );
	
	static void			GetWndToModelMtx		( USAffine2D& wndToModel );
	static void			GetWndToNormMtx			( const USViewport& viewport, USAffine2D& wndToNorm );
	static void			GetWndToWorldMtx		( USAffine2D& wndToWorld );
	static void			GetWndToWorldMtx		( const USViewport& viewport, const USAffine2D& camera, USAffine2D& wndToWorld );
	
	
	
	static void			GetWorldToModelMtx		( USAffine2D& worldToModel );
	static void			GetWorldToWndMtx		( USAffine2D& worldToWnd, float xScale = 1.0f, float yScale = -1.0f );
	static void			GetWorldToWndMtx		( const USViewport& viewport, const USAffine2D& camera, USAffine2D& worldToWnd );
	
	
	
	
	static void			LoadMatrix				( const USMatrix3D& mtx );
	static void			LoadMatrix				( const USAffine2D& mtx );
	static void			MultMatrix				( const USMatrix3D& mtx );
	static void			MultMatrix				( const USAffine2D& mtx );
	static void			SetPen					( u32 penColor, u32 penSize );
	static void			SetPenColor				( u32 penColor );
	static void			SetPenColor				( const USColorVec& color );
	static void			SetPenSize				( u32 penSize );
	static void			SetScissorRect			();
	static void			SetScissorRect			( USRect& rect );
	static void			SetScreenSpace			();
	static void			SetScreenSpace			( USViewport& viewport );
	static void			SetTexture				();
	static void			SetTexture				( USTexture& texture );
	static void			SetUVMtx				();
	static void			SetUVMtx				( const USAffine2D& mtx );
	static void			SetViewProjMtx			();
	static void			SetViewProjMtx			( const USAffine2D& mtx );
	static void			SetViewProjMtx			( const USMatrix3D& mtx );
	static void			SetWorldMtx				();
	static void			SetWorldMtx				( const USAffine2D& mtx );
	static void			SetWorldMtx				( const USMatrix3D& mtx );
};

#endif
