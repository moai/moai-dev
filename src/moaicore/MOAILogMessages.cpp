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
		
		log.RegisterLogMessage ( MOAI_BadCast_DS,						MOAILogMgr::LOG_ERROR,		"Bad cast at position %d: unexpected '%s'" );
		log.RegisterLogMessage ( MOAI_FileNotFound_S,					MOAILogMgr::LOG_ERROR,		"File not found: %s" );
		log.RegisterLogMessage ( MOAI_FileOpenError_S,			MOAILogMgr::LOG_ERROR,	"Couldn't open file: '%s'" );
		log.RegisterLogMessage ( MOAI_FunctionDeprecated_S,				MOAILogMgr::LOG_WARNING,	"WARNING: Function '%s' has been deprecated." );
		log.RegisterLogMessage ( MOAI_IndexNoReserved,					MOAILogMgr::LOG_ERROR,		"Nothing reserved" );
		log.RegisterLogMessage ( MOAI_IndexOutOfRange_DDD,				MOAILogMgr::LOG_ERROR,		"Index %d is out of acceptable range [%d, %d]" );
		log.RegisterLogMessage ( MOAI_NewIsUnsupported,					MOAILogMgr::LOG_ERROR,		"Method \'new\' is unsupported. Instances of this class are created by the engine or through another interface." );
		log.RegisterLogMessage ( MOAI_ParamTypeMismatch,				MOAILogMgr::LOG_ERROR,		"Param type mismatch: check function call" );
		log.RegisterLogMessage ( MOAI_ParamTypeMismatch_DSS,			MOAILogMgr::LOG_ERROR,		"Param type mismatch at position %d: expected a '%s' but got a '%s'" );
		log.RegisterLogMessage ( MOAIAction_Profile_PSFF,				MOAILogMgr::LOG_STATUS,		"MOAIAction::Update(%p: %s [%s]) step %.2f ms took %.2f ms" );
		log.RegisterLogMessage ( MOAIBox2DBody_InvalidVertexCount_D,	MOAILogMgr::LOG_ERROR,		"BOX2D ERROR: Vertex count %d is invalid (less than 3 or greater than max)" );
		log.RegisterLogMessage ( MOAIBox2DBody_MissingInstance,			MOAILogMgr::LOG_ERROR,		"BOX2D ERROR: Attempt to access missing Box2D body instance" );
		log.RegisterLogMessage ( MOAIBox2DFixture_MissingInstance,		MOAILogMgr::LOG_ERROR,		"BOX2D ERROR: Attempt to access missing Box2D fixture instance" );
		log.RegisterLogMessage ( MOAIBox2DJoint_MissingInstance,		MOAILogMgr::LOG_ERROR,		"BOX2D ERROR: Attempt to access missing Box2D joint instance" );
		log.RegisterLogMessage ( MOAIBox2DWorld_IsLocked,				MOAILogMgr::LOG_ERROR,		"BOX2D ERROR: Attempt to perform illegal operation during collision update" );
		log.RegisterLogMessage ( MOAIGfxDevice_OpenGLError_S,			MOAILogMgr::LOG_ERROR,		"OPENGL ERROR: %s" );
		log.RegisterLogMessage ( MOAIGfxResource_MissingDevice,			MOAILogMgr::LOG_ERROR,		"Unable to bind graphics resource - missing graphics device" );
		log.RegisterLogMessage ( MOAINode_AttributeNotFound,			MOAILogMgr::LOG_ERROR,		"No such attribute" );
		log.RegisterLogMessage ( MOAIShader_ShaderInfoLog_S,			MOAILogMgr::LOG_ERROR,		"%s" );
		log.RegisterLogMessage ( MOAITexture_MemoryUse_SDFS,			MOAILogMgr::LOG_STATUS,		"TEXTURE: '%s' %10lu = %6.2fMB < %s" );
		log.RegisterLogMessage ( MOAITexture_NoFramebuffer,				MOAILogMgr::LOG_ERROR,		"TEXTURE: OpenGL framebuffer object is unsupported on this device" );
		log.RegisterLogMessage ( MOAITexture_NonPowerOfTwo_SDD,			MOAILogMgr::LOG_WARNING,	"TEXTURE: '%s' is not a power of two (w:%d, h:%d)" );
	
	#endif
}

//----------------------------------------------------------------//
void MOAILogMessages::RegisterLogMessageIDs ( MOAILuaState& state ) {
	UNUSED ( state );
	
	REGISTER_LOG_MESSAGE ( MOAI_BadCast_DS )
	REGISTER_LOG_MESSAGE ( MOAI_FileNotFound_S )
	REGISTER_LOG_MESSAGE ( MOAI_FunctionDeprecated_S )
	REGISTER_LOG_MESSAGE ( MOAI_IndexNoReserved )
	REGISTER_LOG_MESSAGE ( MOAI_IndexOutOfRange_DDD )
	REGISTER_LOG_MESSAGE ( MOAI_NewIsUnsupported )
	REGISTER_LOG_MESSAGE ( MOAI_ParamTypeMismatch )
	REGISTER_LOG_MESSAGE ( MOAI_ParamTypeMismatch_DSS )
	REGISTER_LOG_MESSAGE ( MOAIAction_Profile_PSFF )
	REGISTER_LOG_MESSAGE ( MOAIBox2DBody_InvalidVertexCount_D )
	REGISTER_LOG_MESSAGE ( MOAIBox2DBody_MissingInstance )
	REGISTER_LOG_MESSAGE ( MOAIBox2DFixture_MissingInstance )
	REGISTER_LOG_MESSAGE ( MOAIBox2DJoint_MissingInstance )
	REGISTER_LOG_MESSAGE ( MOAIBox2DWorld_IsLocked )
	REGISTER_LOG_MESSAGE ( MOAIGfxDevice_OpenGLError_S )
	REGISTER_LOG_MESSAGE ( MOAIGfxResource_MissingDevice )
	REGISTER_LOG_MESSAGE ( MOAINode_AttributeNotFound )
	REGISTER_LOG_MESSAGE ( MOAIShader_ShaderInfoLog_S )
	REGISTER_LOG_MESSAGE ( MOAITexture_MemoryUse_SDFS )
	REGISTER_LOG_MESSAGE ( MOAITexture_NoFramebuffer )
	REGISTER_LOG_MESSAGE ( MOAITexture_NonPowerOfTwo_SDD )
}

