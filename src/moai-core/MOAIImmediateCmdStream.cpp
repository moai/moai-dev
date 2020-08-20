// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractCmdHandler.h>
#include <moai-core/MOAIImmediateCmdStream.h>

//================================================================//
// MOAIImmediateCmdStream
//================================================================//

//----------------------------------------------------------------//
MOAIImmediateCmdStream::MOAIImmediateCmdStream () {

	RTTI_BEGIN ( MOAIImmediateCmdStream )
		RTTI_EXTEND ( MOAIAbstractCmdStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIImmediateCmdStream::~MOAIImmediateCmdStream () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIImmediateCmdStream::MOAIAbstractCmdStream_RetainObject ( MOAILuaObject* object ) {
	UNUSED ( object );
}

//----------------------------------------------------------------//
void MOAIImmediateCmdStream::MOAIAbstractCmdStream_SubmitCommand ( MOAIAbstractCmdHandler& handler, u32 cmd, const void* param, ZLSize paramSize ) {
	UNUSED ( paramSize );
	handler.HandleCommand ( cmd, param );
}
