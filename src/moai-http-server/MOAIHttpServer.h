// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHTTPSERVER_H
#define MOAIHTTPSERVER_H

#include <mongoose.h>

//================================================================//
// MOAIHttpServer
//================================================================//
class MOAIHttpServer :
	public MOAIInstanceEventSource {
private:
	
	mg_context*		mContext;
	
	//----------------------------------------------------------------//
	static int		_start					( lua_State* L );
	static int		_stop					( lua_State* L );
	
	//----------------------------------------------------------------//
	static int		_beginRequest			( mg_connection* connection );
	static void		_endRequest				( const mg_connection* connection, int reply_status_code );
	static int		_logMessage				( const mg_connection* connection, const char* message );

	//----------------------------------------------------------------//
	int				HandleRequest			( mg_connection* connection, mg_request_info* info );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIHttpServer )

	enum {
		EVENT_HANDLE_REQUEST,
		TOTAL_EVENTS,
	};

	//----------------------------------------------------------------//
					MOAIHttpServer			();
					~MOAIHttpServer			();
	void			Start					( cc8** options );
	void			Stop					();
};

#endif
