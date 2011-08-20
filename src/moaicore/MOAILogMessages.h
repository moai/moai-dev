// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOGMESSAGES_H
#define	MOAILOGMESSAGES_H

#include <moaicore/MOAILogMgr.h>

#define REGISTER_LOG_MESSAGE(messageID) state.SetField ( -1, #messageID, ( u32 )messageID );

#ifdef _DEBUG
	#define MOAI_LUA_SETUP(type,str)									\
		USLuaState state ( L );											\
		if ( !state.CheckParams ( 1, str )) {							\
			MOAILog ( L, MOAILogMessages::MOAI_ParamTypeMismatch );		\
			return 0;													\
		}																\
		type* self = state.GetLuaObject < type >( 1 );					\
		if ( !self ) return 0;
#else
	#define MOAI_LUA_SETUP(type,str)									\
		USLuaState state ( L );											\
		type* self = state.GetLuaObject < type >( 1 );					\
		if ( !self ) return 0;
#endif

//================================================================//
// MOAILogMessages
//================================================================//
class MOAILogMessages {
public:

	enum {
		MOAI_FileNotFound_S,
		MOAI_IndexNoReserved,
		MOAI_IndexOutOfRange_DDD,
		MOAI_NewIsUnsupported,
		MOAI_ParamTypeMismatch,
		
		MOAIAction_Profile_PSFF,		// "MOAIAction::Update(%p: %s) step %.2f ms took %.2f ms\n"
		
		MOAINode_AttributeNotFound,
		
		MOAITexture_MemoryUse_SDFS,		// "TEXTURE: %s %10lu = %6.2fMB < %s\n"
	};
	
	//----------------------------------------------------------------//
	static int _alertNewIsUnsupported ( lua_State* L ) {

		MOAILog ( L, MOAI_NewIsUnsupported );
		return 0;
	}
	
	//----------------------------------------------------------------//
	static bool CheckFileExists ( cc8* filename, lua_State* L = 0 ) {
		
		if ( USFileSys::CheckFileExists ( filename )) {
			return true;
		}
		
		STLString expand = USFileSys::Expand ( filename );
		MOAILog ( L, MOAILogMessages::MOAI_FileNotFound_S, expand.str ());
		
		return false;
	}
	
	//----------------------------------------------------------------//
	static bool CheckIndex ( u32 idx, u32 size, lua_State* L = 0 ) {
	
		if ( size == 0 ) {
			MOAILog ( L, MOAILogMessages::MOAI_IndexNoReserved );
			return false;
		}
		else if ( !( idx < size )) {
			MOAILog ( L, MOAILogMessages::MOAI_IndexOutOfRange_DDD, idx, 0, size );
			return false;
		}
		return true;
	}
	
	//----------------------------------------------------------------//
	static bool CheckReserve ( u32 idx, u32 size, lua_State* L = 0 ) {
	
		if ( !( idx < size )) {
			MOAILog ( L, MOAILogMessages::MOAI_IndexNoReserved );
			return false;
		}
		return true;
	}
	
	//----------------------------------------------------------------//
	static void RegisterLogMessages ( USLuaState& state ) {
		UNUSED ( state );
		
		#ifdef _DEBUG
			REGISTER_LOG_MESSAGE ( MOAI_FileNotFound_S )
			REGISTER_LOG_MESSAGE ( MOAI_IndexNoReserved )
			REGISTER_LOG_MESSAGE ( MOAI_IndexOutOfRange_DDD )
			REGISTER_LOG_MESSAGE ( MOAI_NewIsUnsupported )
			REGISTER_LOG_MESSAGE ( MOAI_ParamTypeMismatch )
			
			REGISTER_LOG_MESSAGE ( MOAINode_AttributeNotFound )
		#endif
	}
};

#endif
