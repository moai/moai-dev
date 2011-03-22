// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USCamera2D.h>
#include <uslsext/USCanvas.h>
#include <uslsext/USViewport.h>
#include <uslsext/USViewQuad.h>

//================================================================//
// USCamera2D
//================================================================//

//----------------------------------------------------------------//
void USCamera2D::GetViewQuad ( USViewport& viewport, USViewQuad& viewQuad ) {

	USAffine2D viewProjInv;
	USCanvas::GetViewProjMtxInv ( viewport, *this, viewProjInv );

	USVec2D* points = viewQuad.mPoints;

	points [ 0 ].Init ( -1.0f, 1.0f );
	points [ 1 ].Init ( 1.0f, 1.0f );
	points [ 2 ].Init ( 1.0f, -1.0f );
	points [ 3 ].Init ( -1.0f, -1.0f );
	
	viewQuad.Transform ( viewProjInv );
}

//----------------------------------------------------------------//
void USCamera2D::SerializeIn ( USLuaState& state ) {
	
	this->mLoc.mX = state.GetField ( -1, "mLocX", this->mLoc.mX );
	this->mLoc.mY = state.GetField ( -1, "mLocY", this->mLoc.mY );
	
	this->mRoll = state.GetField ( -1, "mRoll", this->mRoll );
	this->mScale = state.GetField ( -1, "mScale", this->mScale );
}

//----------------------------------------------------------------//
void USCamera2D::SerializeOut ( USLuaState& state ) {
	
	state.SetField ( -1, "mLocX", this->mLoc.mX );
	state.SetField ( -1, "mLocY", this->mLoc.mY );
	
	state.SetField ( -1, "mRoll", this->mRoll );
	state.SetField ( -1, "mScale", this->mScale );
}

//----------------------------------------------------------------//
void USCamera2D::SetScale ( USVec2D& zoomLoc, float scale ) {
	
	USVec2D d0 = zoomLoc;
	d0.Sub ( this->mLoc );
	d0.Scale ( this->mScale );

	USVec2D d1 = zoomLoc;
	d1.Sub ( this->mLoc );
	d1.Scale ( scale );

	USVec2D zoomOff = d1;
	zoomOff.Sub ( d0 );
	
	zoomOff.Scale ( 1.0f / scale );
	
	this->mLoc.Add ( zoomOff );
	this->mScale = scale;
}

//----------------------------------------------------------------//
USCamera2D::USCamera2D () :
	mLoc ( 0.0f, 0.0f ),
	mRoll ( 0.0f ),
	mScale ( 1.0f ) {
}
