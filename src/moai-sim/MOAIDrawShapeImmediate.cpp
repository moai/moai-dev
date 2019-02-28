// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDrawShapeImmediate.h>
#include <moai-sim/MOAIViewport.h>
#include <moai-sim/MOAIQuadBrush.h>

#define DEFAULT_ELLIPSE_STEPS 64

//================================================================//
// MOAIDrawShapeImmediate
//================================================================//

//----------------------------------------------------------------//
MOAIDrawShapeImmediate::MOAIDrawShapeImmediate () {
}

//----------------------------------------------------------------//
MOAIDrawShapeImmediate::~MOAIDrawShapeImmediate () {
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawShapeImmediate::MOAIAbstractDrawShape_DrawLine ( float x0, float y0, float z0, float x1, float y1, float z1 ) {
	
	ZLGfxStateCache& gfxState = MOAIGfxMgr::Get ().mGfxState;

	gfxState.BeginPrim ( ZGL_PRIM_LINES, 2 );
	
		gfxState.WriteVtx ( x0, y0, z0 );
		gfxState.WritePenColor4b ();
		
		gfxState.WriteVtx ( x1, y1, z1 );
		gfxState.WritePenColor4b ();
	
	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawShapeImmediate::MOAIAbstractDrawShape_DrawPoint ( float x, float y, float z ) {

	ZLGfxStateCache& gfxState = MOAIGfxMgr::Get ().mGfxState;
	UNUSED(z);
	gfxState.BeginPrim ( ZGL_PRIM_POINTS, 1 );
		gfxState.WriteVtx ( x, y, 0.0f );
		gfxState.WritePenColor4b ();
	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawShapeImmediate::MOAIAbstractDrawShape_DrawTriangleFill ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) {

	ZLGfxStateCache& gfxState = MOAIGfxMgr::Get ().mGfxState;

	gfxState.BeginPrim ( ZGL_PRIM_TRIANGLES, 3 );
	
		gfxState.WriteVtx ( v0.mX, v0.mY, v0.mZ );
		gfxState.WritePenColor4b ();
	
		gfxState.WriteVtx ( v1.mX, v1.mY, v1.mZ );
		gfxState.WritePenColor4b ();
	
		gfxState.WriteVtx ( v2.mX, v2.mY, v2.mZ );
		gfxState.WritePenColor4b ();

	gfxState.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawShapeImmediate::MOAIAbstractDrawShape_SetPenColor ( u32 color ) {

	MOAIGfxMgr::Get ().mGfxState.SetPenColor ( color );
}

//----------------------------------------------------------------//
void MOAIDrawShapeImmediate::MOAIAbstractDrawShape_SetPenWidth ( float width ) {

	MOAIGfxMgr::Get ().mGfxState.SetPenWidth ( width );
}
