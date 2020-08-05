// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractDSLStream.h>
#include <moai-core/MOAIExampleInstanceDSL.h>
#include <moai-core/MOAILogMgr.h>

//================================================================//
// MOAIExampleInstanceDSL
//================================================================//

//----------------------------------------------------------------//
MOAIExampleInstanceDSL::MOAIExampleInstanceDSL () {

	RTTI_BEGIN ( MOAIExampleInstanceDSL )
//		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIExampleInstanceDSL >)
		RTTI_EXTEND ( MOAIAbstractDSLInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIExampleInstanceDSL::~MOAIExampleInstanceDSL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIExampleInstanceDSL::MOAIAbstractDSLHandler_SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {
	UNUSED ( param );
	UNUSED ( paramSize );

	switch ( cmd ) {
		case CMD_FOO:
			ZLLog_Status ( "%p: FOO!\n", this );
			break;
		case CMD_BAR:
			ZLLog_Status ( "%p: BAR!\n", this );
			break;
		case CMD_BAZ:
			ZLLog_Status ( "%p: BAZ!\n", this );
			break;
	}
}
