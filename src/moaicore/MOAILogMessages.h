// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOGMESSAGES_H
#define	MOAILOGMESSAGES_H

#include <moaicore/MOAILogMgr.h>

#ifdef ANDROID
#include <android/log.h>
#define ANDROID_PRINT(...) __android_log_print(ANDROID_LOG_INFO, "MoaiLog", __VA_ARGS__);
#endif

#define REGISTER_LOG_MESSAGE(messageID) state.SetField ( -1, #messageID, ( u32 )messageID );

//================================================================//
// MOAILogMessages
//================================================================//
class MOAILogMessages {
public:

	enum {
		MOAI_BadCast_DS,
		MOAI_FileNotFound_S,
		MOAI_FileOpenError_S,
		MOAI_FunctionDeprecated_S,
		MOAI_IndexNoReserved,
		MOAI_IndexOutOfRange_DDD,
		MOAI_NewIsUnsupported,
		MOAI_ParamTypeMismatch,
		MOAI_ParamTypeMismatch_DSS,
		MOAIAction_Profile_PSFF,
		MOAIBox2DBody_InvalidVertexCount_D,
		MOAIBox2DBody_MissingInstance,
		MOAIBox2DFixture_MissingInstance,
		MOAIBox2DJoint_MissingInstance,
		MOAIBox2DWorld_IsLocked,
		MOAIGfxDevice_OpenGLError_S,
		MOAIGfxResource_MissingDevice,
		MOAINode_AttributeNotFound,
		MOAIShader_ShaderInfoLog_S,
		MOAITexture_MemoryUse_SDFS,
		MOAITexture_NoFramebuffer,
		MOAITexture_NonPowerOfTwo_SDD,
	};
	
	//----------------------------------------------------------------//
	static int		_alertNewIsUnsupported			( lua_State* L );

	//----------------------------------------------------------------//
	static bool		CheckFileExists					( cc8* filename, lua_State* L = 0 );
	static bool		CheckIndex						( u32 idx, u32 size, lua_State* L = 0 );
	static bool		CheckIndexPlusOne				( u32 idx, u32 size, lua_State* L = 0 );
	static bool		CheckReserve					( u32 idx, u32 size, lua_State* L = 0 );
	static void		RegisterDefaultLogMessages		();
	static void		RegisterLogMessageIDs			( MOAILuaState& state );
};

#endif
