// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractCmdHandler.h>
#include <moai-core/MOAICmdMedium.h>

//================================================================//
// MOAIAbstractCmdHandler
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractCmdHandler::HandleCommand ( u32 cmd, const void* param ) {
	this->MOAIAbstractCmdHandler_HandleCommand ( cmd, param );
}

//----------------------------------------------------------------//
MOAIAbstractCmdHandler::MOAIAbstractCmdHandler () {

	RTTI_BEGIN ( MOAIAbstractCmdHandler )
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractCmdHandler::~MOAIAbstractCmdHandler () {
}

//----------------------------------------------------------------//
void MOAIAbstractCmdHandler::InitializeMedium ( MOAIAbstractCmdMedium& medium ) {

	return this->MOAIAbstractCmdHandler_InitializeMedium ( medium );
}
