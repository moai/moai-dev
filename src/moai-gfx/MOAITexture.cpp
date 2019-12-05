// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAITexture.h>

//================================================================//
// MOAITexture
//================================================================//

//----------------------------------------------------------------//
MOAITexture::MOAITexture () :
	mWidth ( 0 ),
	mHeight ( 0 ),
	mMinFilter ( MOAITextureFilter::LINEAR ),
	mMagFilter ( MOAITextureFilter::NEAREST ),
	mWrap ( MOAITextureWrap::CLAMP ),
	mTextureSize ( 0 ) {

	RTTI_SINGLE ( MOAIGfxResource )
}

//----------------------------------------------------------------//
MOAITexture::~MOAITexture () {
}

//----------------------------------------------------------------//
void MOAITexture::SetFilter ( MOAITextureFilter::Type filter ) {

	this->SetFilter ( filter, filter );
}

//----------------------------------------------------------------//
void MOAITexture::SetFilter ( MOAITextureFilter::Type min, MOAITextureFilter::Type mag ) {

	this->mMinFilter = min;
	this->mMagFilter = mag;
	
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAITexture::SetWrap ( MOAITextureWrap::Type wrap ) {

	this->mWrap = wrap;
	
	this->ScheduleForGPUUpdate ();
}
