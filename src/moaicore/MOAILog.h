// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOG_H
#define	MOAILOG_H

#ifdef _DEBUG

	#ifdef __GNUC__
		
		#define MOAI_LOG(messageID...) \
			MOAILog::Get ().Log ( MOAILog::LOG_STATUS, messageID );
		
		#define MOAI_WARN(state,messageID...) \
			MOAILog::Get ().Log ( MOAILog::LOG_WARNING, state, messageID );
			
		#define MOAI_ERROR(state,messageID...) \
			MOAILog::Get ().Log ( MOAILog::LOG_ERROR, state, messageID );
		
	#else
	
		#define MOAI_LOG(messageID,...) \
			MOAILog::Get ().Log ( MOAILog::LOG_STATUS, messageID, __VA_ARGS__ );
		
		#define MOAI_WARN(state,messageID,...) \
			MOAILog::Get ().Log ( MOAILog::LOG_WARNING, state, messageID, __VA_ARGS__ );
			
		#define MOAI_ERROR(state,messageID,...) \
			MOAILog::Get ().Log ( MOAILog::LOG_ERROR, state, messageID, __VA_ARGS__ );
		
	#endif
	
#else
	
	#ifdef __GNUC__
		#define MOAI_LOG(messageID...)
		#define MOAI_WARN(clause,messageID...)
		#define MOAI_ERROR(clause,messageID...)
	#else
		#define MOAI_LOG(messageID,...)
		#define MOAI_WARN(clause,messageID,...)
		#define MOAI_ERROR(clause,messageID,...)
	#endif

#endif

//================================================================//
// MOAILog
//================================================================//
/**	@brief Singleton for managing debug log messages.
*/
class MOAILog :
	public USGlobalClass < MOAILog, USLuaData > {
private:

	typedef STLMap < u32, STLString >::iterator MessageMapIt;
	STLMap < u32, STLString > mMessageMap;

	u32 mLevel;

	//----------------------------------------------------------------//
	static int		_isDebugBuild			( lua_State* L );
	static int		_log					( lua_State* L );
	static int		_registerLogMessage		( lua_State* L );
	static int		_setLogLevel			( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAILog )
	
	enum {
		LOG_NONE,
		LOG_ERROR,
		LOG_WARNING,
		LOG_STATUS,
	};
	
	//----------------------------------------------------------------//
	void			Log						( u32 level, u32 messageID, ... );
	void			Log						( u32 level, USLuaState& state, u32 messageID, ... );
					MOAILog					();
					~MOAILog				();
	void			RegisterLuaClass		( USLuaState& state );
};

#endif
