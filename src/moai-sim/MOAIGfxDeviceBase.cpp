// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDeviceBase.h>

//================================================================//
// MOAIGfxDeviceBase
//================================================================//

//----------------------------------------------------------------//
MOAIGfxDeviceBase::MOAIGfxDeviceBase () :
	mShaderDirty ( false ),
	mDrawCount ( 0 ) {
	
	this->mViewRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIGfxDeviceBase::~MOAIGfxDeviceBase () {
}
