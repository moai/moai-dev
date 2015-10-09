// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOGMGR_H
#define	MOAILOGMGR_H

#include <moai-core/MOAIGlobals.h>
#include <moai-core/MOAILua.h>

#define MOAI_LUA_SETUP(type,str)												\
	MOAILuaState state ( L );													\
	type* self = MOAILogMgr::Get ().LuaSetup < type >( state, str );			\
	if ( !self ) return 0;

#define MOAI_LUA_SETUP_CLASS(str)												\
	MOAILuaState state ( L );													\
	if ( !MOAILogMgr::Get ().LuaSetupClass ( state, str )) return 0;

#define MOAI_LUA_SETUP_SINGLE(type,str)											\
	MOAILuaState state ( L );													\
	type* self = MOAILogMgr::Get ().LuaSetupSingle < type >( state, str );		\
	if ( !self ) return 0;

//================================================================//
// MOAILogMessage
//================================================================//
class MOAILogMessage {
private:
	
	friend class MOAILogMgr;
	
	u32				mLevel;
	STLString		mFormatString;
};

//================================================================//
// MOAILogMgr
//================================================================//
/**	@lua	MOAILogMgr
	@text	Singleton for managing debug log messages and log level.

	@const LOG_NONE		No logging
	@const LOG_ERROR	Error level
	@const LOG_WARNING	Warning level
	@const LOG_STATUS	Status level
	@const LOG_DEBUG	Debug level
*/
class MOAILogMgr :
	public MOAIGlobalClass < MOAILogMgr, MOAILuaObject > {
private:

	typedef STLMap < u32, MOAILogMessage >::iterator MessageMapIt;
	STLMap < u32, MOAILogMessage > mMessageMap;

	FILE*			mFile;
	bool			mTypeCheckLuaParams;

	//----------------------------------------------------------------//
	static int		_closeFile					( lua_State* L );
	static int		_isDebugBuild				( lua_State* L );
	static int		_log						( lua_State* L );
	static int		_openFile					( lua_State* L );
	static int		_registerLogMessage			( lua_State* L );
	static int		_setLogLevel				( lua_State* L );
	static int		_setTypeCheckLuaParams		( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAILogMgr )
	
	//GET ( FILE*, File, mFile ? mFile : ZLLog::CONSOLE )
	
	//----------------------------------------------------------------//
	void			CloseFile				();
	
	void			LogF					( lua_State *L, u32 level, u32 messageID, ... );
	void			LogF					( lua_State *L, u32 level, cc8* message, ... );
	
	void			LogV					( lua_State *L, u32 level, u32 messageID, va_list args );
	void			LogV					( lua_State *L, u32 level, cc8* message, va_list args );
	
	bool			LuaSetupClass			( MOAILuaState& state, cc8* typeStr );
					MOAILogMgr				();
					~MOAILogMgr				();
	void			OpenFile				( cc8* filename );
	void			RegisterLogMessage		( u32 messageID, u32 level, cc8* formatString );
	void			RegisterLuaClass		( MOAILuaState& state );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* LuaSetup ( MOAILuaState& state, cc8* typeStr ) {
	
		if ( this->mTypeCheckLuaParams && typeStr ) {
			if ( !state.CheckParams ( 1, typeStr, true )) return 0;
		}
		return state.GetLuaObject < TYPE >( 1, true );
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* LuaSetupSingle ( MOAILuaState& state, cc8* typeStr ) {
	
		if ( this->mTypeCheckLuaParams && typeStr ) {
			if ( !state.CheckParams ( 1, typeStr, true )) return 0;
		}
		return MOAIGlobalsMgr::Get ()->GetGlobal < TYPE >();
	}
};

#define MOAILogF(L,level,messageID,...)			MOAILogMgr::Get ().LogF ( L, level, messageID, ##__VA_ARGS__ )
#define MOAILogV(L,level,messageID,args)		MOAILogMgr::Get ().LogV ( L, level, messageID, args )

#endif
