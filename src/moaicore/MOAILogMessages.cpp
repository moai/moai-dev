// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMgr.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAILogMessages::_alertNewIsUnsupported ( lua_State* L ) {

	MOAILog ( L, MOAI_NewIsUnsupported );
	return 0;
}

//================================================================//
// MOAILogMessages
//================================================================//
	
//----------------------------------------------------------------//
bool MOAILogMessages::CheckFileExists ( cc8* filename, lua_State* L ) {
	
	if ( USFileSys::CheckFileExists ( filename )) {
		return true;
	}
	
	STLString expand = USFileSys::GetAbsoluteFilePath ( filename );
	MOAILog ( L, MOAILogMessages::MOAI_FileNotFound_S, expand.str ());
	
	return false;
}

//----------------------------------------------------------------//
bool MOAILogMessages::CheckIndex ( u32 idx, u32 size, lua_State* L ) {

	if ( size == 0 ) {
		MOAILog ( L, MOAILogMessages::MOAI_IndexNoReserved );
		return false;
	}
	else if ( !( idx < size )) {
		MOAILog ( L, MOAILogMessages::MOAI_IndexOutOfRange_DDD, idx, 0, size - 1 );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAILogMessages::CheckIndexPlusOne ( u32 idx, u32 size, lua_State* L ) {

	if ( size == 0 ) {
		MOAILog ( L, MOAILogMessages::MOAI_IndexNoReserved );
		return false;
	}
	else if ( !( idx < size )) {
		MOAILog ( L, MOAILogMessages::MOAI_IndexOutOfRange_DDD, idx + 1, 1, size );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAILogMessages::CheckReserve ( u32 idx, u32 size, lua_State* L ) {

	if ( !( idx < size )) {
		MOAILog ( L, MOAILogMessages::MOAI_IndexNoReserved );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAILogMessages::RegisterDefaultLogMessages () {
	
	#ifndef MOAI_NO_DEFAULT_LOG_MESSAGES
	
		MOAILogMgr& log = MOAILogMgr::Get ();
		
		log.RegisterLogMessage ( MOAI_FileNotFound_S,			MOAILogMgr::LOG_ERROR,		"File not found: %s" );
		log.RegisterLogMessage ( MOAI_IndexNoReserved,			MOAILogMgr::LOG_ERROR,		"Nothing reserved" );
		log.RegisterLogMessage ( MOAI_IndexOutOfRange_DDD,		MOAILogMgr::LOG_ERROR,		"Index %d is out of acceptable range [%d, %d]" );
		log.RegisterLogMessage ( MOAI_NewIsUnsupported,			MOAILogMgr::LOG_ERROR,		"Method \'new\' is unsupported. Instances of this class are created by the engine or through another interface." );
		log.RegisterLogMessage ( MOAI_ParamTypeMismatch,		MOAILogMgr::LOG_ERROR,		"Param type mismatch; check function call" );
		log.RegisterLogMessage ( MOAIAction_Profile_PSFF,		MOAILogMgr::LOG_STATUS,		"MOAIAction::Update(%p: %s) step %.2f ms took %.2f ms" );
		log.RegisterLogMessage ( MOAIGfxDevice_OpenGLError_S,	MOAILogMgr::LOG_ERROR,		"OPENGL ERROR: %s" );
		log.RegisterLogMessage ( MOAINode_AttributeNotFound,	MOAILogMgr::LOG_ERROR,		"No such attribute" );
		log.RegisterLogMessage ( MOAIShader_ShaderInfoLog_S,	MOAILogMgr::LOG_ERROR,		"%s" );
		log.RegisterLogMessage ( MOAITexture_MemoryUse_SDFS,	MOAILogMgr::LOG_STATUS,		"TEXTURE: %s %10lu = %6.2fMB < %s" );
	
	#endif
}

//----------------------------------------------------------------//
void MOAILogMessages::RegisterLogMessageIDs ( USLuaState& state ) {
	UNUSED ( state );
	
	REGISTER_LOG_MESSAGE ( MOAI_FileNotFound_S )
	REGISTER_LOG_MESSAGE ( MOAI_IndexNoReserved )
	REGISTER_LOG_MESSAGE ( MOAI_IndexOutOfRange_DDD )
	REGISTER_LOG_MESSAGE ( MOAI_NewIsUnsupported )
	REGISTER_LOG_MESSAGE ( MOAI_ParamTypeMismatch )
	
	REGISTER_LOG_MESSAGE ( MOAINode_AttributeNotFound )
}

