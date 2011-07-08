// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOGMESSAGES_H
#define	MOAILOGMESSAGES_H

#include <moaicore/MOAILogMgr.h>

#define REGISTER_LOG_MESSAGE(messageID) state.SetField ( -1, #messageID, ( u32 )messageID );

#define MOAI_CHECK_FILE(filename)														\
	if ( !USFileSys::CheckFileExists ( filename )) {									\
		STLString expand = USFileSys::Expand ( filename );								\
		MOAI_ERROR ( state, MOAILogMessages::MOAI_FileNotFound_S, expand.str ());		\
		return 0;																		\
	}

#define MOAI_CHECK_INDEX(idx,size)														\
	if ( size == 0 ) {																	\
		MOAI_ERROR ( state, MOAILogMessages::MOAI_IndexNoReserved );					\
		return 0;																		\
	}																					\
	else if ( !( idx < size )) {														\
		MOAI_ERROR ( state, MOAILogMessages::MOAI_IndexOutOfRange_DDD, idx, 0, size );	\
		return 0;																		\
	}

#define MOAI_CHECK_RESERVE(size)														\
	if ( !( idx < size )) {																\
		MOAI_ERROR ( state, MOAILogMessages::MOAI_IndexNoReserved );					\
		return 0;																		\
	}

#ifdef _DEBUG
	#define MOAI_LUA_SETUP(type,str)														\
		USLuaState state ( L );																\
		if ( !state.CheckParams ( 1, str )) {												\
			MOAI_ERROR ( state, MOAILogMessages::MOAI_ParamTypeMismatch );					\
			return 0;																		\
		}																					\
		type* self = state.GetLuaObject < type >( 1 );										\
		if ( !self ) return 0;
#else
	#define MOAI_LUA_SETUP(type,str)														\
		USLuaState state ( L );																\
		type* self = state.GetLuaObject < type >( 1 );										\
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
		
		MOAINode_AttributeNotFound,
	};
	
	//----------------------------------------------------------------//
	static int _alertNewIsUnsupported ( lua_State* L ) {

		USLuaState state ( L );
		MOAI_ERROR ( state, MOAI_NewIsUnsupported );
		
		return 0;
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
