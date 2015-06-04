// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILogMgr.h>
#include <moai-core/MOAILogMessages.h>

#include <moai-core/MOAILuaState-impl.h>
#include <moai-core/MOAILuaClass-impl.h>

//================================================================//
// helpers
//================================================================//

//----------------------------------------------------------------//
void MOAILog ( lua_State *L, u32 messageID, ... ) {

	if ( MOAILogMgr::IsValid ()) {

		va_list args;
		va_start ( args, messageID );

		MOAILogMgr::Get ().LogVar ( L, messageID, args );

		va_end ( args );
	}
}

//----------------------------------------------------------------//
void MOAIPrint ( cc8* message, ... ) {

	if ( MOAILogMgr::IsValid ()) {

		va_list args;
		va_start ( args, message );

		MOAILogMgr::Get ().PrintVar ( message, args );

		va_end ( args );
	}
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	closeFile
	@text	Resets log output to stdout.

	@out	nil
*/
int MOAILogMgr::_closeFile ( lua_State* L ) {
	UNUSED ( L );

	MOAILogMgr::Get ().CloseFile ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	isDebugBuild
	@text	Returns a boolean value indicating whether Moai has been
			compiles as a debug build or not.

	@out	boolean isDebugBuild
*/
int MOAILogMgr::_isDebugBuild ( lua_State* L ) {

	MOAILuaState state ( L );

	#ifdef _DEBUG
		lua_pushboolean ( state, true );
	#else
		lua_pushboolean ( state, false );
	#endif

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	log
	@text	Alias for print.

	@in		string message
	@out	nil
*/
int MOAILogMgr::_log ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 idx = 1;
	u32 level = LOG_STATUS;

	if ( state.IsType ( 1, LUA_TNUMBER )) {
		level = state.GetValue < u32 >( 1, LOG_STATUS );
		idx++;
	}
	
	cc8* str1 =  state.GetValue < cc8* >( idx++, "" );
	cc8* str2 =  state.GetValue < cc8* >( idx, 0 );

	cc8* msg = str1;
	cc8* token = "MOAI";
	
	if ( str2 ) {
		token = str1;
		msg = str2;
	}

	STLString log;
	log.write ( "[%s] %s", token, msg );

	ZLLog::LogF ( ZLLog::CONSOLE, "%s", log.c_str ());	// Caller's string may contain % and should NOT be used as a format to LogF

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	openFile
	@text	Opens a new file to receive log messages.

	@in		string filename
	@out	nil
*/
int MOAILogMgr::_openFile ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "S" )) return 0;

	cc8* filename = state.GetValue < cc8* >( 1, "" );
	MOAILogMgr::Get ().OpenFile ( filename );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	registerLogMessage
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

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;

	u32 messageID		= state.GetValue < u32 >( 1, 0 );
	cc8* formatString	= state.GetValue < cc8* >( 2, "" );
	u32 level			= state.GetValue < u32 >( 3, LOG_STATUS );

	MOAILogMgr::Get ().RegisterLogMessage ( messageID, level, formatString );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setLogLevel
	@text	Set the logging level.

	@in		number logLevel		One of MOAILogMgr LOG_NONE, LOG_ERROR, LOG_WARNING, LOG_STATUS
	@out	nil
*/
int MOAILogMgr::_setLogLevel ( lua_State* L ) {

	MOAILuaState state ( L );

	u32 level = state.GetValue < u32 >( 1, LOG_NONE );
	MOAILogMgr::Get ().mLevel = level;

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setTypeCheckLuaParams
	@text	Set or clear type checking of parameters passed to Lua bound Moai API functions.

	@opt	boolean check		Default value is false.
	@out	nil
*/
int MOAILogMgr::_setTypeCheckLuaParams ( lua_State* L ) {

	MOAILuaState state ( L );

	bool check = state.GetValue < bool >( 1, false );
	MOAILogMgr::Get ().mTypeCheckLuaParams = check;

	return 0;
}

//================================================================//
// MOAILogMgr
//================================================================//

//----------------------------------------------------------------//
void MOAILogMgr::CloseFile () {

	if ( this->mOwnsFileHandle ) {
		fclose ( this->mFile );
		this->mFile = 0;
		this->mOwnsFileHandle = false;
	}
}

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
					this->Print ( "----------------------------------------------------------------\n" );
				}

				this->PrintVar ( message.mFormatString, args );
				
				if ( L ) {
					this->Print ( "\n" );
					MOAILuaState state ( L );
					state.PrintStackTrace ( this->mFile, NULL, 0 );
					this->Print ( "\n" );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
bool MOAILogMgr::LuaSetupClass( MOAILuaState& state, cc8* typeStr ) {

	if ( this->mTypeCheckLuaParams && typeStr ) {
		return state.CheckParams ( 1, typeStr, true );
	}
	return false;
}

//----------------------------------------------------------------//
MOAILogMgr::MOAILogMgr () :
	mLevel ( LOG_STATUS ),
	mFile ( 0 ),
	mOwnsFileHandle ( false ),
	mTypeCheckLuaParams ( true ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAILogMgr::~MOAILogMgr () {

	this->CloseFile ();
}

//----------------------------------------------------------------//
void MOAILogMgr::OpenFile ( cc8* filename ) {

	FILE* file = fopen ( filename, "w" );
	if ( file ) {
		this->CloseFile ();
		this->mFile = file;
		this->mOwnsFileHandle = true;
	}
}

//----------------------------------------------------------------//
void MOAILogMgr::Print ( cc8* message, ... ) {

	va_list args;
	va_start ( args, message );

	MOAILogMgr::Get ().PrintVar ( message, args );

	va_end ( args );
}

//----------------------------------------------------------------//
void MOAILogMgr::PrintVar ( cc8* message, va_list args ) {

	ZLLog::LogV ( this->mFile, message, args );
}

//----------------------------------------------------------------//
void MOAILogMgr::RegisterLogMessage ( u32 messageID, u32 level, cc8* formatString ) {

	if ( strlen ( formatString )) {

		MOAILogMessage& message = MOAILogMgr::Get ().mMessageMap [ messageID ];

		message.mLevel = level;
		message.mFormatString = formatString;
	}
	else {
		MOAILogMgr::Get ().mMessageMap.erase ( messageID );
	}
}

//----------------------------------------------------------------//
void MOAILogMgr::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "LOG_NONE",			( u32 )LOG_NONE );
	state.SetField ( -1, "LOG_ERROR",			( u32 )LOG_ERROR );
	state.SetField ( -1, "LOG_WARNING",			( u32 )LOG_WARNING );
	state.SetField ( -1, "LOG_STATUS",			( u32 )LOG_STATUS );
	state.SetField ( -1, "LOG_DEBUG",			( u32 )LOG_DEBUG );

	MOAILogMessages::RegisterLogMessageIDs ( state );

	luaL_Reg regTable[] = {
		{ "closeFile",					_closeFile },
		{ "log",						_log },
		{ "isDebugBuild",				_isDebugBuild },
		{ "openFile",					_openFile },
		{ "registerLogMessage",			_registerLogMessage },
		{ "setLogLevel",				_setLogLevel },
		{ "setTypeCheckLuaParams",		_setTypeCheckLuaParams },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}



