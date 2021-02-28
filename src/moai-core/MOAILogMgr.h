// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOGMGR_H
#define	MOAILOGMGR_H

#include <moai-core/MOAILua.h>
#include <moai-core/strings.h>

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
	public virtual ZLContextClass,
	public virtual MOAILuaObject {
private:

	FILE*			mFile;
	bool			mTypeCheckLuaParams;

	//----------------------------------------------------------------//
	static int		_closeFile					( lua_State* L );
	static int		_isDebugBuild				( lua_State* L );
	static int		_log						( lua_State* L );
	static int		_openFile					( lua_State* L );
	static int		_setLogLevel				( lua_State* L );
	static int		_setTypeCheckLuaParams		( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_SINGLETON ( MOAILogMgr )
	
	//GET ( FILE*, File, mFile ? mFile : ZLLog::CONSOLE )
	
	//----------------------------------------------------------------//
	static bool		CheckFileExists			( cc8* filename, lua_State* L = 0 );
	static bool		CheckIndex				( size_t idx, size_t size, lua_State* L = 0 );
	static bool		CheckIndexPlusOne		( size_t idx, size_t size, lua_State* L = 0 );
	static bool		CheckReserve			( size_t idx, size_t size, lua_State* L = 0 );
	void			CloseFile				();
	static void		LogF					( lua_State *L, u32 level, cc8* message, ... );
	static void		LogV					( lua_State *L, u32 level, cc8* message, va_list args );
	bool			LuaSetupClass			( MOAILuaState& state, cc8* typeStr );
					MOAILogMgr				( ZLContext& context );
					~MOAILogMgr				();
	void			OpenFile				( cc8* filename );
};

#define MOAILogF(L,level,message,...)		MOAILogMgr::LogF ( L, level, message, ##__VA_ARGS__ )
#define MOAILogV(L,level,message,args)		MOAILogMgr::LogV ( L, level, message, args )

#endif
