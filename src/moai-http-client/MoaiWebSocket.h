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
// MOAIWebSocket
//================================================================//
/**	@name	MOAIWebSocket
	@text	Represents one websocket instance. Registers to the manager for polling.
*/

class MOAIWebSocketMgr;

class MOAIWebSocket :
	public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int		_start				( lua_State* L );
	static int		_write				( lua_State* L );
	static int		_setListener	( lua_State* L );
	
	// instance private
	void notifyError(const char *error);
	
	// events
	void on_open(websocketpp::connection_hdl hdl);
	void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
	void on_fail( websocketpp::connection_hdl handle );
	void on_close( websocketpp::connection_hdl handle );

	MOAILuaLocal								mOnCallback;
	client											mClient;
	websocketpp::connection_hdl mHandle;

	friend class MOAIWebSocketMgr;

public:
	
	DECL_LUA_FACTORY ( MOAIWebSocket )

	enum {
		ON_CONNECT,
		ON_FAIL,
		ON_MESSAGE,
		ON_CLOSE,
		TOTAL,
	};
	
	MOAILuaRef		mListeners [ TOTAL ];

	//----------------------------------------------------------------//
					MOAIWebSocket				();
					~MOAIWebSocket			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );

	void			Process					();

};

#endif
