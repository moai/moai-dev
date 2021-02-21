// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAITexture.h>

//================================================================//
// MOAITexture
//================================================================//

//----------------------------------------------------------------//
MOAITexture::MOAITexture () :
	mMinFilter ( MOAITextureFilterEnum::LINEAR ),
	mMagFilter ( MOAITextureFilterEnum::NEAREST ),
	mWrap ( MOAITextureWrapEnum::CLAMP ),
	mTextureSize ( 0 ) {
	
	RTTI_BEGIN ( MOAITexture )
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture::~MOAITexture () {
}

//----------------------------------------------------------------//
void MOAITexture::SetFilter ( MOAITextureFilterEnum::_ filter ) {

	this->SetFilter ( filter, filter );
}

//----------------------------------------------------------------//
void MOAITexture::SetFilter ( MOAITextureFilterEnum::_ min, MOAITextureFilterEnum::_ mag ) {

	this->mMinFilter = min;
	this->mMagFilter = mag;
	
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAITexture::SetWrap ( MOAITextureWrapEnum::_ wrap ) {

	this->mWrap = wrap;
	
	this->ScheduleForGPUUpdate ();
}
