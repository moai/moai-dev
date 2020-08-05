// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractDSLInterface.h>
#include <moai-core/MOAIImmediateDSLStream.h>

//================================================================//
// MOAIImmediateDSLStream
//================================================================//

//----------------------------------------------------------------//
MOAIImmediateDSLStream::MOAIImmediateDSLStream () {

	RTTI_BEGIN ( MOAIRetainedDSLStream )
		RTTI_EXTEND ( MOAIAbstractDSLStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIImmediateDSLStream::~MOAIImmediateDSLStream () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIImmediateDSLStream::MOAIAbstractDSLHandler_SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {

	assert ( this->mDSLInterface );
	this->mDSLInterface->SubmitCommand ( cmd, param, paramSize );
}
