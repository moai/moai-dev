// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIWEBSOCKET_H
#define MOAIWEBSOCKET_H

#include <moai-core/MOAILua.h>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> client;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

//================================================================//
// MOAIFoo
//================================================================//
/**	@name	MOAIFoo
	@text	Example class for extending Moai using MOAILuaObject.
			Copy this object, rename it and add your own stuff.
			Just don't forget to register it with the runtime
			using the REGISTER_LUA_CLASS macro (see moaicore.cpp).
*/
class WebSocketWrapper {
	MOAILuaRef 	callback;
};

class MOAIWebSocketMgr;

class MOAIWebSocket :
	public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int		_classHello			( lua_State* L );
	static int		_instanceHello		( lua_State* L );
	static int		_start		( lua_State* L );
	static int		_write		( lua_State* L );

	MOAILuaLocal		mOnCallback;
	client mClient;
	websocketpp::connection_hdl mHandle;
	void on_open(websocketpp::connection_hdl hdl);
	void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
	void on_fail( websocketpp::connection_hdl handle );

	friend class MOAIWebSocketMgr;

public:
	
	DECL_LUA_FACTORY ( MOAIWebSocket )

	//----------------------------------------------------------------//
					MOAIWebSocket				();
					~MOAIWebSocket			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );

	void			Process					();

};

#endif
