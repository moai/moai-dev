// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILogMgr.h>
#include <moai-core/MOAILogMessages.h>
#include <moai-core/MOAILuaState-impl.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAILogMessages::_alertNewIsUnsupported ( lua_State* L ) {

	MOAILogF ( L, ZLLog::LOG_ERROR, MOAI_NewIsUnsupported );
	return 0;
}

//================================================================//
// MOAILogMessages
//================================================================//
	
//----------------------------------------------------------------//
bool MOAILogMessages::CheckFileExists ( cc8* filename, lua_State* L ) {
	
	if ( ZLFileSys::CheckFileExists ( filename )) {
		return true;
	}
	
	STLString expand = ZLFileSys::GetAbsoluteFilePath ( filename );
	MOAILogF ( L, MOAILogMessages::MOAI_FileNotFound_S, expand.str ());
	
	return false;
}

//----------------------------------------------------------------//
bool MOAILogMessages::CheckIndex ( size_t idx, size_t size, lua_State* L ) {

	if ( size == 0 ) {
		MOAILogF ( L, ZLLog::LOG_ERROR, MOAILogMessages::MOAI_IndexNoReserved );
		return false;
	}
	else if ( !( idx < size )) {
		MOAILogF ( L, MOAILogMessages::MOAI_IndexOutOfRange_DDD, ( u32 )idx, 0, ( u32 )( size - 1 ));
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAILogMessages::CheckIndexPlusOne ( size_t idx, size_t size, lua_State* L ) {

	if ( size == 0 ) {
		MOAILogF ( L, ZLLog::LOG_ERROR, MOAILogMessages::MOAI_IndexNoReserved );
		return false;
	}
	else if ( !( idx < size )) {
		MOAILogF ( L, ZLLog::LOG_ERROR, MOAILogMessages::MOAI_IndexOutOfRange_DDD, idx + 1, 1, size );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAILogMessages::CheckReserve ( size_t idx, size_t size, lua_State* L ) {

	if ( !( idx < size )) {
		MOAILogF ( L, ZLLog::LOG_ERROR, MOAILogMessages::MOAI_IndexNoReserved );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAILogMessages::RegisterDefaultLogMessages () {
	
	#ifndef MOAI_NO_DEFAULT_LOG_MESSAGES
	
		MOAILogMgr& log = MOAILogMgr::Get ();
		
		log.RegisterLogMessage ( MOAI_BadCast_DS,						ZLLog::LOG_ERROR,		"Bad cast at position %d: unexpected '%s'\n" );
		log.RegisterLogMessage ( MOAI_FileNotFound_S,					ZLLog::LOG_ERROR,		"File not found: %s\n" );
		log.RegisterLogMessage ( MOAI_FileOpenError_S,					ZLLog::LOG_ERROR,		"Couldn't open file: '%s'\n" );
		log.RegisterLogMessage ( MOAI_FunctionDeprecated_S,				ZLLog::LOG_WARNING,		"WARNING: Function '%s' has been deprecated.\n" );
		log.RegisterLogMessage ( MOAI_IndexNoReserved,					ZLLog::LOG_ERROR,		"Nothing reserved\n" );
		log.RegisterLogMessage ( MOAI_IndexOutOfRange_DDD,				ZLLog::LOG_ERROR,		"Index %d is out of acceptable range [%d, %d]\n" );
		log.RegisterLogMessage ( MOAI_NewIsUnsupported,					ZLLog::LOG_ERROR,		"Method \'new\' is unsupported. Instances of this class are created by the engine or through another interface.\n" );
		log.RegisterLogMessage ( MOAI_ParamTypeMismatch,				ZLLog::LOG_ERROR,		"Param type mismatch: check function call\n" );
		log.RegisterLogMessage ( MOAI_ParamTypeMismatch_DSS,			ZLLog::LOG_ERROR,		"Param type mismatch at position %d: expected a '%s' but got a '%s'\n" );
		log.RegisterLogMessage ( MOAIAction_Profile_PSFF,				ZLLog::LOG_STATUS,		"MOAIAction::Update(%p: %s [%s]) step %.2f ms took %.2f ms\n" );
		log.RegisterLogMessage ( MOAIBox2DBody_InvalidVertexCount_D,	ZLLog::LOG_ERROR,		"BOX2D ERROR: Vertex count %d is invalid (less than 3 or greater than max)\n" );
		log.RegisterLogMessage ( MOAIBox2DBody_MissingInstance,			ZLLog::LOG_ERROR,		"BOX2D ERROR: Attempt to access missing Box2D body instance\n" );
		log.RegisterLogMessage ( MOAIBox2DFixture_MissingInstance,		ZLLog::LOG_ERROR,		"BOX2D ERROR: Attempt to access missing Box2D fixture instance\n" );
		log.RegisterLogMessage ( MOAIBox2DJoint_MissingInstance,		ZLLog::LOG_ERROR,		"BOX2D ERROR: Attempt to access missing Box2D joint instance\n" );
		log.RegisterLogMessage ( MOAIBox2DWorld_IsLocked,				ZLLog::LOG_ERROR,		"BOX2D ERROR: Attempt to perform illegal operation during collision update\n" );
		log.RegisterLogMessage ( MOAIGfxDevice_OpenGLError_S,			ZLLog::LOG_ERROR,		"OPENGL ERROR: %s\n" );
		log.RegisterLogMessage ( MOAIGfxResource_MissingDevice,			ZLLog::LOG_ERROR,		"Unable to bind graphics resource - missing graphics device\n" );
		log.RegisterLogMessage ( MOAINode_AttributeNotFound,			ZLLog::LOG_ERROR,		"No such attribute\n" );
		log.RegisterLogMessage ( MOAIShader_ShaderInfoLog_S,			ZLLog::LOG_ERROR,		"%s\n" );
		log.RegisterLogMessage ( MOAITexture_MemoryUse_SDFS,			ZLLog::LOG_STATUS,		"TEXTURE: '%s' %10luk = %6.2fMB < %s\n" );
		log.RegisterLogMessage ( MOAITexture_NoFramebuffer,				ZLLog::LOG_ERROR,		"TEXTURE: OpenGL framebuffer object is unsupported on this device\n" );
		log.RegisterLogMessage ( MOAITexture_NonPowerOfTwo_SDD,			ZLLog::LOG_WARNING,		"TEXTURE: '%s' is not a power of two (w:%d, h:%d)\n" );
	
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

