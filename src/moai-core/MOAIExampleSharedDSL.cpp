// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractDSLStream.h>
#include <moai-core/MOAIExampleSharedDSL.h>
#include <moai-core/MOAILogMgr.h>

//================================================================//
// MOAIExampleSharedDSL
//================================================================//

//----------------------------------------------------------------//
MOAIExampleSharedDSL::MOAIExampleSharedDSL () {

	RTTI_BEGIN ( MOAIExampleSharedDSL )
		RTTI_EXTEND ( MOAIAbstractDSLInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIExampleSharedDSL::~MOAIExampleSharedDSL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIExampleSharedDSL::MOAIAbstractDSLHandler_SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {
	UNUSED ( param );
	UNUSED ( paramSize );

	switch ( cmd ) {
		case CMD_FOO:
			ZLLog_Status ( "FOO!\n" );
			break;
		case CMD_BAR:
			ZLLog_Status ( "BAR!\n" );
			break;
		case CMD_BAZ:
			ZLLog_Status ( "BAZ!\n" );
			break;
	}
}
