// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/ZLViewport.h>

//================================================================//
// ZLViewport
//================================================================//

//----------------------------------------------------------------//
float ZLViewport::GetAspect () const {
	return this->Width () / this->Height ();
}

//----------------------------------------------------------------//
float ZLViewport::GetInvAspect () const {
	return this->Height () / this->Width ();
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLViewport::GetNormToWndMtx () const {

	ZLMatrix4x4 normToWnd;

	ZLMatrix4x4 mtx;
	ZLRect rect = this->GetRect ();

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Wnd
	normToWnd.Scale ( hWidth, -hHeight, 1.0f );
	
	mtx.Translate ( hWidth + rect.mXMin, hHeight + rect.mYMin, 0.0f );
	normToWnd.Append ( mtx );
	
	return normToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLViewport::GetProjMtx () const {

	ZLMatrix4x4 proj;

	ZLMatrix4x4 mtx;
	ZLRect rect = this->GetRect ();

	// rotate
	proj.RotateZ ( -this->mRotation * ( float )D2R );

	// project
	ZLVec2D viewScale = this->GetScale ();
	float xScale = ( 2.0f / rect.Width ()) * viewScale.mX;
	float yScale = ( 2.0f / rect.Height ()) * viewScale.mY;
	
	mtx.Scale ( xScale, yScale, -1.0f ); // Z must be non-zero to produce invertible projection matrix
	proj.Append ( mtx );
	
	// offset
	mtx.Translate ( this->mOffset.mX, this->mOffset.mY, 0.0f );
	proj.Append ( mtx );
	
	return proj;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLViewport::GetProjMtxInv () const {

	ZLMatrix4x4 projInv;

	ZLMatrix4x4 mtx;
	ZLRect rect = this->GetRect ();

	// Inv Project
	
	// offset
	projInv.Translate ( -this->mOffset.mX, -this->mOffset.mY, 0.0f );
	
	// project
	ZLVec2D viewScale = this->GetScale ();
	float invXScale = 1.0f / (( 2.0f / rect.Width () * viewScale.mX ));
	float invYScale = 1.0f / (( 2.0f / rect.Height () * viewScale.mY ));
	
	mtx.Scale ( invXScale, invYScale, -1.0f );
	projInv.Append ( mtx );
	
	// rotate
	mtx.RotateZ ( this->mRotation * ( float )D2R );
	projInv.Append ( mtx );
	
	return projInv;
}

//----------------------------------------------------------------//
ZLRect ZLViewport::GetRect () const {

	return *this;
}

//----------------------------------------------------------------//
ZLVec2D ZLViewport::GetScale () const {

	ZLVec2D scale ( 1.0f, 1.0f );
	
	if ( this->mXScale && this->mYScale ) {
		scale.mX = this->Width () / this->mScale.mX;
		scale.mY = this->Height () / this->mScale.mY;
	}
	else {
		
		float result = 1.0f;
		
		if ( this->mXScale ) {
			result = (( float )this->Width ()) / this->mScale.mX;
		}
		
		if ( this->mYScale ) {
			result = (( float )this->Height ()) / this->mScale.mY;
		}
		
		scale.mX = result;
		scale.mY = result;
	}
	
	return scale;
}

//----------------------------------------------------------------//
ZLVec2D ZLViewport::GetUnits () const {

	ZLVec2D scale ( 1.0f, 1.0f );
	
	if ( this->mXScale && this->mYScale ) {
		scale.mX = this->mScale.mX;
		scale.mY = this->mScale.mY;
	}
	else {
		
		float result = 1.0f;
		
		if ( this->mXScale ) {
			result = this->mScale.mX;
		}
		
		if ( this->mYScale ) {
			result = this->mScale.mY;
		}
		
		scale.mX = result;
		scale.mY = result;
	}
	
	return scale;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLViewport::GetWndToNormMtx () const {

	ZLMatrix4x4 wndToNorm;

	ZLMatrix4x4 mtx;
	ZLRect rect = this->GetRect ();

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Inv Wnd
	wndToNorm.Translate ( -hWidth - rect.mXMin, -hHeight - rect.mYMin, 0.0f );
	
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ), 1.0f );
	wndToNorm.Append ( mtx );
	
	return wndToNorm;
}

//----------------------------------------------------------------//
void ZLViewport::SetOffset ( float xOffset, float yOffset ) {

	this->mOffset.Init ( xOffset, yOffset );
}

//----------------------------------------------------------------//
void ZLViewport::SetScale ( float xScale, float yScale ) {

	this->mXScale = ( xScale != 0.0f );
	this->mScale.mX = xScale;
	
	this->mYScale = ( yScale != 0.0f );
	this->mScale.mY = yScale;
}

//----------------------------------------------------------------//
ZLViewport::ZLViewport () :
	mXScale ( true ),
	mYScale ( true ),
	mScale ( 1.0f, 1.0f ),
	mOffset ( 0.0f, 0.0f ),
	mRotation ( 0.0f ) {
	
	this->Init ( 0.0f, 0.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
ZLViewport::~ZLViewport () {
}
