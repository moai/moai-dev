// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIFrameBuffer.h>

//================================================================//
// ZLShaderGL
//================================================================//

//----------------------------------------------------------------//
MOAIFrameBuffer::MOAIFrameBuffer () :
	mBufferWidth ( 0 ),
	mBufferHeight ( 0 ),
	mBufferScale ( 1.0 ),
	mLandscape ( false ),
	mNeedsClear ( true ) {
	
	RTTI_BEGIN ( MOAIFrameBuffer )
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBuffer::~MOAIFrameBuffer () {
}

//----------------------------------------------------------------//
bool MOAIFrameBuffer::NeedsClear () const {

	return this->mNeedsClear;
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::NeedsClear ( bool needsClear ) {

	this->mNeedsClear = needsClear;
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::SetBufferSize ( u32 width, u32 height ) {

	this->mBufferWidth = width;
	this->mBufferHeight = height;
}

//----------------------------------------------------------------//
ZLRect MOAIFrameBuffer::WndRectToDevice ( ZLRect rect ) const {

	rect.Bless ();

	if ( this->mLandscape ) {
	
		float width = ( float )this->mBufferWidth;
		
		float xMin = rect.mYMin;
		float yMin = width - rect.mXMax;
		float xMax = rect.mYMax;
		float yMax = width - rect.mXMin;
		
		rect.mXMin = xMin;
		rect.mYMin = yMin;
		rect.mXMax = xMax;
		rect.mYMax = yMax;
	}
	else {
	
		float height = ( float )this->mBufferHeight;
		
		float xMin = rect.mXMin;
		float yMin = height - rect.mYMax;
		float xMax = rect.mXMax;
		float yMax = height - rect.mYMin;
		
		rect.mXMin = xMin;
		rect.mYMin = yMin;
		rect.mXMax = xMax;
		rect.mYMax = yMax;
	}

	rect.Scale ( this->mBufferScale, this->mBufferScale );
	return rect;
}
