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
void MOAIDrawShapeImmediate::MOAIDrawShape_DrawLine ( float x0, float y0, float z0, float x1, float y1, float z1 ) {
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mVertexCache.BeginPrim ( ZGL_PRIM_LINES, 2 );
	
		gfxMgr.mVertexCache.WriteVtx ( x0, y0, z0 );
		gfxMgr.mVertexCache.WritePenColor4b ();
		
		gfxMgr.mVertexCache.WriteVtx ( x1, y1, z1 );
		gfxMgr.mVertexCache.WritePenColor4b ();
	
	gfxMgr.mVertexCache.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawShapeImmediate::MOAIDrawShape_DrawPoint ( float x, float y, float z ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mVertexCache.BeginPrim ( ZGL_PRIM_POINTS, 1 );
		gfxMgr.mVertexCache.WriteVtx ( x, y, 0.0f );
		gfxMgr.mVertexCache.WritePenColor4b ();
	gfxMgr.mVertexCache.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawShapeImmediate::MOAIDrawShape_DrawTriangleFill ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mVertexCache.BeginPrim ( ZGL_PRIM_TRIANGLES, 3 );
	
		gfxMgr.mVertexCache.WriteVtx ( v0.mX, v0.mY, v0.mZ );
		gfxMgr.mVertexCache.WritePenColor4b ();
	
		gfxMgr.mVertexCache.WriteVtx ( v1.mX, v1.mY, v1.mZ );
		gfxMgr.mVertexCache.WritePenColor4b ();
	
		gfxMgr.mVertexCache.WriteVtx ( v2.mX, v2.mY, v2.mZ );
		gfxMgr.mVertexCache.WritePenColor4b ();

	gfxMgr.mVertexCache.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawShapeImmediate::MOAIDrawShape_SetPenColor ( u32 color ) {

	MOAIGfxMgr::Get ().mGfxState.SetPenColor ( color );
}

//----------------------------------------------------------------//
void MOAIDrawShapeImmediate::MOAIDrawShape_SetPenWidth ( float width ) {

	MOAIGfxMgr::Get ().mGfxState.SetPenWidth ( width );
}
