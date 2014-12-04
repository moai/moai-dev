// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIQuadBrush
//================================================================//

//----------------------------------------------------------------//
void MOAIQuadBrush::BindVertexFormat ( MOAIGfxDevice& gfxDevice ) {
	
	gfxDevice.SetVertexPreset ( MOAIVertexFormatMgr::XYZWUVC );
	gfxDevice.SetPrimType ( ZGL_PRIM_TRIANGLES );
}

//----------------------------------------------------------------//
void MOAIQuadBrush::Draw () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.WriteQuad ( this->mModelQuad.mV, this->mUVQuad.mV );
}

//----------------------------------------------------------------//
void MOAIQuadBrush::Draw ( float xOff, float yOff, float zOff ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.WriteQuad ( this->mModelQuad.mV, this->mUVQuad.mV, xOff, yOff, zOff ); 
}

//----------------------------------------------------------------//
void MOAIQuadBrush::Draw ( float xOff, float yOff, float zOff, float xScale, float yScale ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.WriteQuad ( this->mModelQuad.mV, this->mUVQuad.mV, xOff, yOff, zOff, xScale, yScale ); 
}

//----------------------------------------------------------------//
void MOAIQuadBrush::Draw ( float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.WriteQuad ( this->mModelQuad.mV, this->mUVQuad.mV, xOff, yOff, zOff, xScale, yScale, uOff, vOff, uScale, vScale ); 
}

//----------------------------------------------------------------//
ZLRect MOAIQuadBrush::GetUVBounds () {

	ZLRect rect;
	
	rect.Init ( this->mUVQuad.mV [ 0 ]);
	rect.Grow ( this->mUVQuad.mV [ 1 ]);
	rect.Grow ( this->mUVQuad.mV [ 2 ]);
	rect.Grow ( this->mUVQuad.mV [ 3 ]);
	
	return rect;
}

//----------------------------------------------------------------//
bool MOAIQuadBrush::GetUVForCartesian ( u32 triangleID, float x, float y, ZLVec2D& uv ) {
	
	return ZLQuad::RemapCoord ( this->mModelQuad, this->mUVQuad, triangleID, x, y, uv );
}

//----------------------------------------------------------------//
ZLRect MOAIQuadBrush::GetVtxBounds () {

	ZLRect rect;
	
	rect.Init ( this->mModelQuad.mV [ 0 ]);
	rect.Grow ( this->mModelQuad.mV [ 1 ]);
	rect.Grow ( this->mModelQuad.mV [ 2 ]);
	rect.Grow ( this->mModelQuad.mV [ 3 ]);
	
	return rect;
}

//----------------------------------------------------------------//
void MOAIQuadBrush::ScaleUVs ( float xScale, float yScale ) {

	this->mUVQuad.mV [ 0 ].Scale ( xScale, yScale );
	this->mUVQuad.mV [ 1 ].Scale ( xScale, yScale );
	this->mUVQuad.mV [ 2 ].Scale ( xScale, yScale );
	this->mUVQuad.mV [ 3 ].Scale ( xScale, yScale );
}

//----------------------------------------------------------------//
void MOAIQuadBrush::ScaleVerts ( float xScale, float yScale ) {

	this->mModelQuad.mV [ 0 ].Scale ( xScale, yScale );
	this->mModelQuad.mV [ 1 ].Scale ( xScale, yScale );
	this->mModelQuad.mV [ 2 ].Scale ( xScale, yScale );
	this->mModelQuad.mV [ 3 ].Scale ( xScale, yScale );
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetUVs ( const ZLRect& rect ) {
	
	// left top
	this->mUVQuad.mV [ 0 ].mX = rect.mXMin;
	this->mUVQuad.mV [ 0 ].mY = rect.mYMax;
	
	// right top
	this->mUVQuad.mV [ 1 ].mX = rect.mXMax;
	this->mUVQuad.mV [ 1 ].mY = rect.mYMax;
	
	// right bottom
	this->mUVQuad.mV [ 2 ].mX = rect.mXMax;
	this->mUVQuad.mV [ 2 ].mY = rect.mYMin;
	
	// left bottom
	this->mUVQuad.mV [ 3 ].mX = rect.mXMin;
	this->mUVQuad.mV [ 3 ].mY = rect.mYMin;
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetUVs ( float* buffer ) {

	// left top
	this->mUVQuad.mV [ 0 ].mX = buffer [ 0 ];
	this->mUVQuad.mV [ 0 ].mY = buffer [ 1 ];
	
	// right top
	this->mUVQuad.mV [ 1 ].mX = buffer [ 2 ];
	this->mUVQuad.mV [ 1 ].mY = buffer [ 3 ];
	
	// right bottom
	this->mUVQuad.mV [ 2 ].mX = buffer [ 4 ];
	this->mUVQuad.mV [ 2 ].mY = buffer [ 5 ];
	
	// left bottom
	this->mUVQuad.mV [ 3 ].mX = buffer [ 6 ];
	this->mUVQuad.mV [ 3 ].mY = buffer [ 7 ];
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetUVs ( float x0, float y0, float x1, float y1 ) {
	
	// left top
	this->mUVQuad.mV [ 0 ].mX = x0;
	this->mUVQuad.mV [ 0 ].mY = y1;
	
	// right top
	this->mUVQuad.mV [ 1 ].mX = x1;
	this->mUVQuad.mV [ 1 ].mY = y1;
	
	// right bottom
	this->mUVQuad.mV [ 2 ].mX = x1;
	this->mUVQuad.mV [ 2 ].mY = y0;
	
	// left bottom
	this->mUVQuad.mV [ 3 ].mX = x0;
	this->mUVQuad.mV [ 3 ].mY = y0;
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetUVs ( const ZLVec2D& v0, const ZLVec2D& v1,const  ZLVec2D& v2, const ZLVec2D& v3 ) {

	// left top
	this->mUVQuad.mV [ 0 ] = v0;
	
	// right top
	this->mUVQuad.mV [ 1 ] = v1;
	
	// right bottom
	this->mUVQuad.mV [ 2 ] = v2;
	
	// left bottom
	this->mUVQuad.mV [ 3 ] = v3;
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetUVs ( float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3 ) {

	// left top
	this->mUVQuad.mV [ 0 ].mX = x0;
	this->mUVQuad.mV [ 0 ].mY = y0;
	
	// right top
	this->mUVQuad.mV [ 1 ].mX = x1;
	this->mUVQuad.mV [ 1 ].mY = y1;
	
	// right bottom
	this->mUVQuad.mV [ 2 ].mX = x2;
	this->mUVQuad.mV [ 2 ].mY = y2;
	
	// left bottom
	this->mUVQuad.mV [ 3 ].mX = x3;
	this->mUVQuad.mV [ 3 ].mY = y3;
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetVerts ( const ZLRect& rect ) {
	
	// left top
	this->mModelQuad.mV [ 0 ].mX = rect.mXMin;
	this->mModelQuad.mV [ 0 ].mY = rect.mYMax;
	
	// right top
	this->mModelQuad.mV [ 1 ].mX = rect.mXMax;
	this->mModelQuad.mV [ 1 ].mY = rect.mYMax;
	
	// right bottom
	this->mModelQuad.mV [ 2 ].mX = rect.mXMax;
	this->mModelQuad.mV [ 2 ].mY = rect.mYMin;
	
	// left bottom
	this->mModelQuad.mV [ 3 ].mX = rect.mXMin;
	this->mModelQuad.mV [ 3 ].mY = rect.mYMin;
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetVerts ( float* buffer ) {

	// left top
	this->mModelQuad.mV [ 0 ].mX = buffer [ 0 ];
	this->mModelQuad.mV [ 0 ].mY = buffer [ 1 ];
	
	// right top
	this->mModelQuad.mV [ 1 ].mX = buffer [ 2 ];
	this->mModelQuad.mV [ 1 ].mY = buffer [ 3 ];
	
	// right bottom
	this->mModelQuad.mV [ 2 ].mX = buffer [ 4 ];
	this->mModelQuad.mV [ 2 ].mY = buffer [ 5 ];
	
	// left bottom
	this->mModelQuad.mV [ 3 ].mX = buffer [ 6 ];
	this->mModelQuad.mV [ 3 ].mY = buffer [ 7 ];
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetVerts ( float x0, float y0, float x1, float y1 ) {
	
	// left top
	this->mModelQuad.mV [ 0 ].mX = x0;
	this->mModelQuad.mV [ 0 ].mY = y1;
	
	// right top
	this->mModelQuad.mV [ 1 ].mX = x1;
	this->mModelQuad.mV [ 1 ].mY = y1;
	
	// right bottom
	this->mModelQuad.mV [ 2 ].mX = x1;
	this->mModelQuad.mV [ 2 ].mY = y0;
	
	// left bottom
	this->mModelQuad.mV [ 3 ].mX = x0;
	this->mModelQuad.mV [ 3 ].mY = y0;
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetVerts ( const ZLVec2D& v0, const ZLVec2D& v1, const ZLVec2D& v2, const ZLVec2D& v3 ) {

	// left top
	this->mModelQuad.mV [ 0 ] = v0;
	
	// right top
	this->mModelQuad.mV [ 1 ] = v1;
	
	// right bottom
	this->mModelQuad.mV [ 2 ] = v2;
	
	// left bottom
	this->mModelQuad.mV [ 3 ] = v3;
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetVerts ( float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3 ) {

	// left top
	this->mUVQuad.mV [ 0 ].mX = x0;
	this->mUVQuad.mV [ 0 ].mY = y0;
	
	// right top
	this->mUVQuad.mV [ 1 ].mX = x1;
	this->mUVQuad.mV [ 1 ].mY = y1;
	
	// right bottom
	this->mUVQuad.mV [ 2 ].mX = x2;
	this->mUVQuad.mV [ 2 ].mY = y2;
	
	// left bottom
	this->mUVQuad.mV [ 3 ].mX = x3;
	this->mUVQuad.mV [ 3 ].mY = y3;
}

//----------------------------------------------------------------//
void MOAIQuadBrush::SetVerts ( const ZLVec2D& v0, float radius ) {

	this->SetVerts (
		v0.mX - radius,
		v0.mY - radius,
		v0.mX + radius,
		v0.mY + radius
	);
}

//----------------------------------------------------------------//
void MOAIQuadBrush::TransformVerts ( const ZLAffine3D& mtx ) {
	
	mtx.Transform ( this->mModelQuad.mV [ 0 ]);
	mtx.Transform ( this->mModelQuad.mV [ 1 ]);
	mtx.Transform ( this->mModelQuad.mV [ 2 ]);
	mtx.Transform ( this->mModelQuad.mV [ 3 ]);
}

//----------------------------------------------------------------//
void MOAIQuadBrush::TransformUVs ( const ZLAffine3D& mtx ) {

	mtx.Transform ( this->mUVQuad.mV [ 0 ]);
	mtx.Transform ( this->mUVQuad.mV [ 1 ]);
	mtx.Transform ( this->mUVQuad.mV [ 2 ]);
	mtx.Transform ( this->mUVQuad.mV [ 3 ]);
}
