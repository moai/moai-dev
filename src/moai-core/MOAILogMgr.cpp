// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILogMgr.h>

#include <moai-core/MOAILuaState-impl.h>
#include <moai-core/MOAILuaClass-impl.h>

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

	@overload
		@in		string message
		@out	nil
	
	@overload
		@in		number level	Default: LOG_STATUS
		@in		string message
		@out	nil
	
	@overload
		@in		number level	Default: LOG_STATUS
		@in		string token
		@in		string message
		@out	nil
*/
int MOAILogMgr::_log ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 idx = 1;
	u32 level = ZLLog::LOG_STATUS;

	if ( state.IsType ( 1, LUA_TNUMBER )) {
		level = state.GetValue < u32 >( 1, ZLLog::LOG_STATUS );
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
	log.write ( "[%s-%d] %s", token, level, msg );

	ZLLog::Get ().LogF ( level, ZLLog::CONSOLE, "%s", log.c_str ());	// Caller's string may contain % and should NOT be used as a format to LogF

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
/**	@lua	setLogLevel
	@text	Set the logging level.

	@in		number logLevel		One of MOAILogMgr LOG_NONE, LOG_ERROR, LOG_WARNING, LOG_STATUS
	@out	nil
*/
int MOAILogMgr::_setLogLevel ( lua_State* L ) {

	MOAILuaState state ( L );

	u32 level = state.GetValue < u32 >( 1, ZLLog::LOG_NONE );
	ZLLog::Get ().SetLogLevel ( level );

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
bool MOAILogMgr::CheckFileExists ( cc8* filename, lua_State* L ) {
	
	if ( ZLFileSys::CheckFileExists ( filename )) {
		return true;
	}
	
	STLString expand = ZLFileSys::GetAbsoluteFilePath ( filename );
	MOAILogF ( L, ZLLog::LOG_ERROR, MOAISTRING_FileNotFound_S, expand.str ());
	
	return false;
}

//----------------------------------------------------------------//
bool MOAILogMgr::CheckIndex ( size_t idx, size_t size, lua_State* L ) {

	if ( size == 0 ) {
		MOAILogF ( L, ZLLog::LOG_ERROR, MOAISTRING_IndexNoReserved );
		return false;
	}
	else if ( !( idx < size )) {
		MOAILogF ( L, ZLLog::LOG_ERROR, MOAISTRING_IndexOutOfRange_DDD, ( u32 )idx, 0, ( u32 )( size - 1 ));
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAILogMgr::CheckIndexPlusOne ( size_t idx, size_t size, lua_State* L ) {

	if ( size == 0 ) {
		MOAILogF ( L, ZLLog::LOG_ERROR, MOAISTRING_IndexNoReserved );
		return false;
	}
	else if ( !( idx < size )) {
		MOAILogF ( L, ZLLog::LOG_ERROR, MOAISTRING_IndexOutOfRange_DDD, idx + 1, 1, size );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAILogMgr::CheckReserve ( size_t idx, size_t size, lua_State* L ) {

	if ( !( idx < size )) {
		MOAILogF ( L, ZLLog::LOG_ERROR, MOAISTRING_IndexNoReserved );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAILogMgr::CloseFile () {

	if ( this->mFile ) {
		fclose ( this->mFile );
		this->mFile = 0;
		ZLLog::Get ().SetRedirect ( 0 );
	}
}

//----------------------------------------------------------------//
void MOAILogMgr::LogF ( lua_State *L, u32 level, cc8* message, ... ) {

	va_list args;
	va_start ( args, message );

	this->LogV ( L, level, message, args );

	va_end ( args );
}

//----------------------------------------------------------------//
void MOAILogMgr::LogV ( lua_State *L, u32 level, cc8* message, va_list args ) {

	if ( ZLLog::Get ().IsEnabled ( level )) {

		if ( L ) {
			ZLLog::Get ().LogF ( level, ZLLog::CONSOLE, "----------------------------------------------------------------\n" );
		}

		ZLLog::Get ().LogV ( level, ZLLog::CONSOLE, message, args );
		
		size_t msgSize = strlen ( message );
		if ( msgSize && ( message [ msgSize - 1 ] != '\n' )) {
			ZLLog::Get ().LogF ( level, ZLLog::CONSOLE, "\n" );
		}
		
		if ( L ) {
			ZLLog::Get ().LogF ( level, ZLLog::CONSOLE, "\n" );
			MOAILuaState state ( L );
			state.LogStackTrace ( level, ZLLog::CONSOLE, NULL, 0 );
			ZLLog::Get ().LogF ( level, ZLLog::CONSOLE, "\n" );
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
	mFile ( 0 ),
	mTypeCheckLuaParams ( true ) {
	
	RTTI_BEGIN ( MOAILogMgr )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAILogMgr >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAILogMgr::~MOAILogMgr () {

	this->CloseFile ();
}

//----------------------------------------------------------------//
void MOAILogMgr::OpenFile ( cc8* filename ) {

	this->CloseFile ();

	FILE* file = fopen ( filename, "w" );
	if ( file ) {
		this->mFile = file;
		ZLLog::Get ().SetRedirect ( file );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAILogMgr::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	state.SetField ( -1, "LOG_DEBUG",			( u32 )ZLLog::LOG_DEBUG );
	state.SetField ( -1, "LOG_STATUS",			( u32 )ZLLog::LOG_STATUS );
	state.SetField ( -1, "LOG_WARNING",			( u32 )ZLLog::LOG_WARNING );
	state.SetField ( -1, "LOG_ERROR",			( u32 )ZLLog::LOG_ERROR );
	state.SetField ( -1, "LOG_FATAL",			( u32 )ZLLog::LOG_FATAL );
	state.SetField ( -1, "LOG_NONE",			( u32 )ZLLog::LOG_NONE );
	state.SetField ( -1, "LOG_REPORT",			( u32 )ZLLog::LOG_REPORT );

	luaL_Reg regTable[] = {
		{ "closeFile",					_closeFile },
		{ "log",						_log },
		{ "isDebugBuild",				_isDebugBuild },
		{ "openFile",					_openFile },
		{ "setLogLevel",				_setLogLevel },
		{ "setTypeCheckLuaParams",		_setTypeCheckLuaParams },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
