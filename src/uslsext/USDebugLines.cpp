// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslsext/USCanvas.h>
#include <uslsext/USDebugLines.h>
#include <uslsext/USDrawBuffer.h>
#include <uslsext/USMathConsts.h>

#define MAX_LINES 1024

//================================================================//
// USDebugLine
//================================================================//

//----------------------------------------------------------------//
void USDebugLine::Draw () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	drawBuffer.SetPenColor ( this->mColor );
	drawBuffer.SetPenWidth ( this->mWidth );

	drawBuffer.BeginPrim ();
	
		drawBuffer.WriteVtx ( this->mVtx [ 0 ]);
		drawBuffer.WritePenColor ();
		
		drawBuffer.WriteVtx ( this->mVtx [ 1 ]);
		drawBuffer.WritePenColor ();
	
	drawBuffer.EndPrim ();
}

//----------------------------------------------------------------//
void USDebugLine::SetVerts ( float x0, float y0, float x1, float y1 ) {

	this->mVtx [ 0 ].mX = x0;
	this->mVtx [ 0 ].mY = y0;
	
	this->mVtx [ 1 ].mX = x1;
	this->mVtx [ 1 ].mY = y1;
}

//================================================================//
// USDebugLines
//================================================================//

//----------------------------------------------------------------//
void USDebugLines::Draw () {

	if ( !this->mTop ) return;
	
	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	
	drawBuffer.BindTexture ();
	drawBuffer.SetBlendMode ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	drawBuffer.SetPrimType ( GL_LINES );
	drawBuffer.BindVertexPreset ( USVertexFormatMgr::VTX_FMT_XYC );
	
	drawBuffer.SetVtxTransform ();
	
	for ( u32 i = 0; i < this->mTop; ++i ) {
		this->mLineBuffer [ i ].Draw ();
	}
}

//----------------------------------------------------------------//
void USDebugLines::DrawEllipse ( USVec2D& loc, float xRad, float yRad, u32 steps ) {

	static const u32 MAX = 64;
	if ( steps > MAX ) steps = MAX;
	USVec2D vtx [ MAX * 2 ];
	
	glVertexPointer ( 2, GL_FLOAT, 0, vtx );
	glEnableClientState ( GL_VERTEX_ARRAY );

	float angle = ( float )TWOPI / ( float )steps;
	float angleStep = ( float )PI;
	
	for ( u32 i = 0; i < steps; ++i, angleStep += angle ) {
		
		vtx [ i ].mX = loc.mX + ( Sin ( angleStep ) * xRad );
		vtx [ i ].mY = loc.mY + ( Cos ( angleStep ) * yRad );
	}
	
	for ( u32 i = 0; i < steps; ++i ) {
		
		USVec2D& v0 = vtx [ i ];
		USVec2D& v1 = vtx [( i + 1 ) % steps ];
		
		this->DrawLine ( v0.mX, v0.mY, v1.mX, v1.mY );
	}
}

//----------------------------------------------------------------//
void USDebugLines::DrawLine ( const USVec2D& v0, const USVec2D& v1 ) {

	this->DrawLine ( v0.mX, v0.mY, v1.mX, v1.mY );
}

//----------------------------------------------------------------//
void USDebugLines::DrawLine ( float x0, float y0, float x1, float y1 ) {

	USDebugLine* line = this->NextLine ();
	if ( line ) {

		if ( this->mPenSpace == MODEL_SPACE ) {

			USVec2D v0 ( x0, y0 );
			USVec2D v1 ( x1, y1 );

			this->mModelToWorldMtx.Transform ( v0 );
			this->mModelToWorldMtx.Transform ( v1 );
	
			line->SetVerts ( v0.mX, v0.mY, v1.mX, v1.mY );
		}
		else {
		
			line->SetVerts ( x0, y0, x1, y1 );
		}
		
		line->mColor = this->mPenColor;
		line->mWidth = ( float )this->mPenWidth;
	}
}

//----------------------------------------------------------------//
void USDebugLines::DrawRect ( const USRect& rect ) {

	this->DrawLine ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMin );
	this->DrawLine ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMax );
	this->DrawLine ( rect.mXMin, rect.mYMin, rect.mXMin, rect.mYMax );
	this->DrawLine ( rect.mXMax, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void USDebugLines::DrawQuad ( const USQuad& quad ) {

	this->DrawLine ( quad.mV [ 0 ].mX, quad.mV [ 0 ].mY, quad.mV [ 1 ].mX, quad.mV [ 1 ].mY );
	this->DrawLine ( quad.mV [ 1 ].mX, quad.mV [ 1 ].mY, quad.mV [ 2 ].mX, quad.mV [ 2 ].mY );
	this->DrawLine ( quad.mV [ 2 ].mX, quad.mV [ 2 ].mY, quad.mV [ 3 ].mX, quad.mV [ 3 ].mY );
	this->DrawLine ( quad.mV [ 3 ].mX, quad.mV [ 3 ].mY, quad.mV [ 0 ].mX, quad.mV [ 0 ].mY );
}

//----------------------------------------------------------------//
USDebugLine* USDebugLines::NextLine () {

	if ( !this->mLineBuffer.Size ()) {
		this->mLineBuffer.Init ( MAX_LINES );
	}

	if ( this->mTop < this->mLineBuffer.Size ()) {
		return &this->mLineBuffer [ this->mTop++ ];
	}
	return 0;
}

//----------------------------------------------------------------//
void USDebugLines::Reset () {

	this->mTop = 0;
	this->mModelToWorldMtx.Ident ();
	this->SetPen ( 0xffffffff, 1.0f, MODEL_SPACE );
}

//----------------------------------------------------------------//
void USDebugLines::SetWorldMtx () {

	this->mModelToWorldMtx.Ident ();
}

//----------------------------------------------------------------//
void USDebugLines::SetWorldMtx ( const USAffine2D& mtx ) {

	this->mModelToWorldMtx = mtx;
}

//----------------------------------------------------------------//
void USDebugLines::SetPen ( u32 penColor, float penWidth, u32 penSpace ) {

	this->mPenColor = penColor;
	this->mPenWidth = ( u32 )penWidth;
	this->mPenSpace = penSpace;
}

//----------------------------------------------------------------//
USDebugLines::USDebugLines () {

	this->Reset ();
}

//----------------------------------------------------------------//
USDebugLines::~USDebugLines () {
}
