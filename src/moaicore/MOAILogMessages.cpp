// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMgr.h>
#include <moaicore/MOAILogMessages.h>

#define REGISTER_LOG_MESSAGE(messageID) state.SetField ( -1, #messageID, ( u32 )messageID );

//================================================================//
// MOAILogMessages
//================================================================//

//----------------------------------------------------------------//
void MOAILogMessages::RegisterLogMessages ( USLuaState& state ) {
	
	#ifdef _DEBUG
	
		REGISTER_LOG_MESSAGE ( MOAI_IndexOutOfRange_DDD )
	
	#endif
}
