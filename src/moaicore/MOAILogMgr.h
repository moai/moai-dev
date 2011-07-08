// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOGMGR_H
#define	MOAILOGMGR_H

#ifdef _DEBUG

	#ifdef __GNUC__
		
		#define MOAI_LOG(messageID...)																\
			MOAILogMgr::Get ().Log ( MOAILogMgr::LOG_STATUS, messageID );
		
		#define MOAI_WARN(state,messageID...)														\
			MOAILogMgr::Get ().Log ( MOAILogMgr::LOG_WARNING, state, messageID );
			
		#define MOAI_ERROR(state,messageID...)														\
			MOAILogMgr::Get ().Log ( MOAILogMgr::LOG_ERROR, state, messageID );
		
		#define MOAI_ERROR_UNLESS(clause,state,messageID...)										\
			if (( clause ) == false ) {																\
				MOAILogMgr::Get ().Log ( MOAILogMgr::LOG_ERROR, state, messageID );					\
			}
		
	#else
	
		#define MOAI_LOG(messageID,...)																\
			MOAILogMgr::Get ().Log ( MOAILogMgr::LOG_STATUS, messageID, __VA_ARGS__ );
		
		#define MOAI_WARN(state,messageID,...)														\
			MOAILogMgr::Get ().Log ( MOAILogMgr::LOG_WARNING, state, messageID, __VA_ARGS__ );
			
		#define MOAI_ERROR(state,messageID,...)														\
			MOAILogMgr::Get ().Log ( MOAILogMgr::LOG_ERROR, state, messageID, __VA_ARGS__ );
		
		#define MOAI_ERROR_UNLESS(clause,state,messageID,...)										\
			if (( clause ) == false ) {																\
				MOAILogMgr::Get ().Log ( MOAILogMgr::LOG_ERROR, state, messageID, __VA_ARGS__ );	\
			}
		
	#endif
	
#else
	
	#ifdef __GNUC__
		#define MOAI_LOG(messageID...)
		#define MOAI_WARN(state,messageID...)
		#define MOAI_ERROR(state,messageID...)
		#define MOAI_ERROR_UNLESS(clause,state,messageID...)
	#else
		#define MOAI_LOG(messageID,...)
		#define MOAI_WARN(state,messageID,...)
		#define MOAI_ERROR(state,messageID,...)
		#define MOAI_ERROR_UNLESS(clause,state,messageID,...)
	#endif

#endif

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

	typedef STLMap < u32, STLString >::iterator MessageMapIt;
	STLMap < u32, STLString > mMessageMap;

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
	void			Log						( u32 level, u32 messageID, ... );
	void			Log						( u32 level, USLuaState& state, u32 messageID, ... );
					MOAILogMgr				();
					~MOAILogMgr				();
	void			RegisterLuaClass		( USLuaState& state );
};

#endif
