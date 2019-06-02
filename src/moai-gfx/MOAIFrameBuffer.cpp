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

	RTTI_SINGLE ( MOAILuaObject )
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
