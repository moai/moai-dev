// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILineBrush.h>

//================================================================//
// MOAILineBrush
//================================================================//

//----------------------------------------------------------------//
void MOAILineBrush::Draw () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.SetPrimType ( GL_LINES );

	gfxDevice.BeginPrim ();
	
		gfxDevice.WriteVtx ( this->mVtx [ 0 ], this->mVtx [ 1 ], 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( this->mVtx [ 2 ], this->mVtx [ 3 ], 0.0f );
		gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAILineBrush::SetVerts ( USVec2D& v0, USVec2D& v1 ) {

	this->mVtx [ 0 ] = v0.mX;
	this->mVtx [ 1 ] = v0.mY;
	
	this->mVtx [ 2 ] = v1.mX;
	this->mVtx [ 3 ] = v1.mY;
}

//----------------------------------------------------------------//
void MOAILineBrush::SetVerts ( float x0, float y0, float x1, float y1 ) {

	this->mVtx [ 0 ] = x0;
	this->mVtx [ 1 ] = y0;
	
	this->mVtx [ 2 ] = x1;
	this->mVtx [ 3 ] = y1;
}
