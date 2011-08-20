// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOGMGR_H
#define	MOAILOGMGR_H

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
/**	@name	MOAILogMgr
	@text	Singleton for managing debug log messages and log level.

	@const LOG_NONE
	@const LOG_ERROR
	@const LOG_WARNING
	@const LOG_STATUS
*/
class MOAILogMgr :
	public USGlobalClass < MOAILogMgr, USLuaObject > {
private:

	typedef STLMap < u32, MOAILogMessage >::iterator MessageMapIt;
	STLMap < u32, MOAILogMessage > mMessageMap;

	u32 mLevel;

	//----------------------------------------------------------------//
	static int		_isDebugBuild				( lua_State* L );
	static int		_log						( lua_State* L );
	static int		_registerLogMessage			( lua_State* L );
	static int		_setLogLevel				( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAILogMgr )
	
	enum {
		LOG_NONE,
		LOG_ERROR,
		LOG_WARNING,
		LOG_STATUS,
	};
	
	//----------------------------------------------------------------//
	void			Log						( lua_State *L, u32 messageID, ... );
	void			LogVar					( lua_State *L, u32 messageID, va_list args );
					MOAILogMgr				();
					~MOAILogMgr				();
	void			RegisterLuaClass		( USLuaState& state );
};

//================================================================//
// helpers
//================================================================//
extern void MOAILog		( lua_State *L, u32 messageID, ... );

#endif
