// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractDSLHandler.h>

//================================================================//
// MOAIAbstractDSLHandler
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDSLHandler::MOAIAbstractDSLHandler () {
}

//----------------------------------------------------------------//
MOAIAbstractDSLHandler::~MOAIAbstractDSLHandler () {
}

//----------------------------------------------------------------//
void MOAIAbstractDSLHandler::RetainObject ( MOAILuaObject* object ) {
	this->MOAIAbstractDSLHandler_RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractDSLHandler::SubmitCommand ( u32 cmd, const void* param, ZLSize size ) {
	this->MOAIAbstractDSLHandler_SubmitCommand ( cmd, param, size );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDSLHandler::MOAIAbstractDSLHandler_RetainObject ( MOAILuaObject* object ) {
	UNUSED ( object );
}
