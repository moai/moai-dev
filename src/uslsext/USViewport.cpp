// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USMathConsts.h>
#include <uslsext/USGfxDevice.h>
#include <uslsext/USViewport.h>

//================================================================//
// USViewport
//================================================================//

//----------------------------------------------------------------//
float USViewport::GetAspect () const {
	return this->Width () / this->Height ();
}

//----------------------------------------------------------------//
float USViewport::GetInvAspect () const {
	return this->Height () / this->Width ();
}

//----------------------------------------------------------------//
USRect USViewport::GetRect () const {

	return *this;
}

//----------------------------------------------------------------//
USVec2D USViewport::GetScale () const {

	USVec2D scale ( 1.0f, 1.0f );
	
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
USVec2D USViewport::GetUnits () const {

	USVec2D scale ( 1.0f, 1.0f );
	
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
void USViewport::SetOffset ( float xOffset, float yOffset ) {

	this->mOffset.Init ( xOffset, yOffset );
}

//----------------------------------------------------------------//
void USViewport::SetRotation ( float degrees ) {

	this->mRotation = degrees;
}

//----------------------------------------------------------------//
void USViewport::SetScale ( float xScale, float yScale ) {

	this->mXScale = ( xScale != 0.0f );
	this->mScale.mX = xScale;
	
	this->mYScale = ( yScale != 0.0f );
	this->mScale.mY = yScale;
}

//----------------------------------------------------------------//
USViewport::USViewport () :
	mXScale ( true ),
	mYScale ( true ),
	mScale ( 1.0f, 1.0f ),
	mOffset ( 0.0f, 0.0f ),
	mRotation ( 0.0f ) {
	
	this->Init ( 0.0f, 0.0f, 1.0f, 1.0f );
}
