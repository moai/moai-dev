// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIFrameBuffer.h>

//================================================================//
// MOAIFrameBuffer
//================================================================//

//----------------------------------------------------------------//
MOAIFrameBuffer::MOAIFrameBuffer ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	mBufferScale ( 1.0 ),
	mLandscape ( false ) {
	
	RTTI_BEGIN ( MOAIFrameBuffer )
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBuffer::~MOAIFrameBuffer () {
}

//----------------------------------------------------------------//
ZLRect MOAIFrameBuffer::WndRectToDevice ( ZLRect rect ) const {

	rect.Bless ();

	if ( this->mLandscape ) {
	
		float width = ( float )this->mWidth;
		
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
	
		float height = ( float )this->mHeight;
		
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
