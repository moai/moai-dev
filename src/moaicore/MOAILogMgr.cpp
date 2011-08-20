// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMgr.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// helpers
//================================================================//

//----------------------------------------------------------------//
void MOAILog ( lua_State *L, u32 messageID, ... ) {
	
	va_list args;
	va_start ( args, messageID );
	
	MOAILogMgr::Get ().LogVar ( L, messageID, args );
	
	va_end ( args );
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	isDebugBuild
	@text	Returns a boolean value indicating whether Moai has been
			compiles as a debug build or not.
	
	@out	boolean isDebugBuild
*/
int MOAILogMgr::_isDebugBuild ( lua_State* L ) {

	USLuaState state ( L );
	
	#ifdef _DEBUG
		lua_pushboolean ( state, true );
	#else
		lua_pushboolean ( state, false );
	#endif
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	log
	@text	Alias for print.
	
	@in		string message
	@out	nil
*/
int MOAILogMgr::_log ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "S" )) return 0;

	cc8* msg = state.GetValue < cc8* >( 1, "" );
	USLog::Print ( msg );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	registerLogMessage
	@text	Register a format string to handle a log message. Register
			an empty string to hide messages.
	
	@in		number messageID
	@opt	string formatString		Default value is an empty string.
	@opt	number level			One of MOAILogMgr.LOG_ERROR, MOAILogMgr.LOG_WARNING, MOAILogMgr.LOG_STATUS.
									Default value is MOAILogMgr.LOG_STATUS.
	@out	nil
*/
int MOAILogMgr::_registerLogMessage ( lua_State* L ) {
	UNUSED ( L );

	#ifdef _DEBUG

		USLuaState state ( L );
		if ( !state.CheckParams ( 1, "N" )) return 0;

		u32 messageID		= state.GetValue < u32 >( 1, 0 );
		cc8* formatString	= state.GetValue < cc8* >( 2, "" );
		u32 level			= state.GetValue < u32 >( 3, LOG_STATUS );

		MOAILogMessage& message = MOAILogMgr::Get ().mMessageMap [ messageID ];
		
		message.mLevel = level;
		message.mFormatString = formatString;
	
	#endif
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLogLevel
	@text	Set the logging level.
	
	@in		number logLevel		One of MOAILogMgr LOG_NONE, LOG_ERROR, LOG_WARNING, LOG_STATUS
	@out	nil
*/
int MOAILogMgr::_setLogLevel ( lua_State* L ) {

	USLuaState state ( L );

	u32 level = state.GetValue < u32 >( 1, LOG_NONE );
	MOAILogMgr::Get ().mLevel = level;
	
	return 0;
}

//================================================================//
// MOAILogMgr
//================================================================//

//----------------------------------------------------------------//
void MOAILogMgr::Log ( lua_State *L, u32 messageID, ... ) {
	
	va_list args;
	va_start ( args, messageID );
	
	MOAILogMgr::Get ().LogVar ( L, messageID, args );
	
	va_end ( args );
}

//----------------------------------------------------------------//
void MOAILogMgr::LogVar ( lua_State *L, u32 messageID, va_list args ) {

	if ( this->mLevel ) {

		MessageMapIt messageMapIt = this->mMessageMap.find ( messageID );
		if ( messageMapIt != this->mMessageMap.end ()) {
		
			MOAILogMessage& message = messageMapIt->second;
			
			if ( message.mLevel <= this->mLevel ) {
				
				if ( L ) {
					printf ( "----------------------------------------------------------------\n" );
				}
				
				vprintf ( message.mFormatString, args );
				printf ( "\n" );
				
				if ( L ) {
					USLuaState state ( L );
					state.PrintStackTrace ( 0 );
					printf ( "\n" );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
MOAILogMgr::MOAILogMgr () :
	mLevel ( LOG_STATUS ) {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAILogMgr::~MOAILogMgr () {
}

//----------------------------------------------------------------//
void MOAILogMgr::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "LOG_NONE",			( u32 )LOG_NONE );
	state.SetField ( -1, "LOG_ERROR",			( u32 )LOG_ERROR );
	state.SetField ( -1, "LOG_WARNING",			( u32 )LOG_WARNING );
	state.SetField ( -1, "LOG_STATUS",			( u32 )LOG_STATUS );
	
	MOAILogMessages::RegisterLogMessages ( state );
	
	luaL_Reg regTable[] = {
		{ "log",					_log },
		{ "isDebugBuild",			_isDebugBuild },
		{ "registerLogMessage",		_registerLogMessage },
		{ "setLogLevel",			_setLogLevel },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}


