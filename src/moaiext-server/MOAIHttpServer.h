// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHTTPSERVER_H
#define MOAIHTTPSERVER_H

#include <moaicore/moaicore.h>
#include <mongoose.h>

//================================================================//
// MOAIHttpServer
//================================================================//
class MOAIHttpServer :
	public virtual MOAILuaObject {
private:
	
	mg_context*		mContext;
	
	//----------------------------------------------------------------//
	static int		_start					( lua_State* L );
	static int		_stop					( lua_State* L );
	
	//----------------------------------------------------------------//
	static int		_beginRequest			( mg_connection* connection );
	static void		_endRequest				( const mg_connection* connection, int reply_status_code );
	static int		_logMessage				( const mg_connection* connection, const char* message );

public:
	
	DECL_LUA_FACTORY ( MOAIHttpServer )

	//----------------------------------------------------------------//
					MOAIHttpServer			();
					~MOAIHttpServer			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Start					( cc8** options );
	void			Stop					();
};

#endif
