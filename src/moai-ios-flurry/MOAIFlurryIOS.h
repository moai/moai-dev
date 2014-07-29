// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFLURRYIOS_H
#define	MOAIFLURRYIOS_H

#import <moai-core/headers.h>

//================================================================//
// MOAIFlurryIOS
//================================================================//
/**	@lua	MOAIFlurryIOS
	@text	Wrapper for Flurry interface.
*/
class MOAIFlurryIOS :
	public MOAIGlobalClass < MOAIFlurryIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_beginEvent							( lua_State* L );
	static int	_endEvent							( lua_State* L );
	static int	_getAgentVersion					( lua_State* L );
	static int	_logEvent							( lua_State* L );
	static int	_setAge								( lua_State* L );
	static int	_setAppVersion						( lua_State* L );
	static int	_setDebugLogEnabled					( lua_State* L );
	static int	_setEventLoggingEnabled				( lua_State* L );
	static int	_setGender							( lua_State* L );
	static int	_setLocation						( lua_State* L );
	static int	_setSecureTransportEnabled			( lua_State* L );
	static int	_setSessionContinueSeconds			( lua_State* L );
	static int	_setSessionReportsOnCloseEnabled	( lua_State* L );
	static int	_setSessionReportsOnPauseEnabled	( lua_State* L );
	static int	_setShowErrorInLogEnabled			( lua_State* L );
	static int	_setUserID							( lua_State* L );
	static int	_startSession						( lua_State* L );

	//----------------------------------------------------------------//
	static void	LogEvent				( lua_State* L, int idx, bool timed );

public:
	
	DECL_LUA_SINGLETON ( MOAIFlurryIOS )
	
	enum {
		GENDER_MALE,
		GENDER_FEMALE,
		GENDER_UNKNOWN,
	};
	
	//----------------------------------------------------------------//
			MOAIFlurryIOS				();
			~MOAIFlurryIOS				();
	void	RegisterLuaClass			( MOAILuaState& state );
};

#endif
