// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-http-client/MOAIWebSocket.h>
#include <moai-http-client/MOAIWebSocketMgr.h>

//#include <chrono>

//================================================================//
// lua
//================================================================//

/**	@name	_start
	@text	Starts webrtc connection.
	@out	zero for no error, -1 for error 
	@out  error string
*/
int MOAIWebSocket::_start ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebSocket, "US" )
	
	self->mClient.clear_access_channels(websocketpp::log::alevel::all);
	self->mClient.clear_error_channels(websocketpp::log::elevel::all);
	
	self->mClient.init_asio();
	self->mClient.set_open_handler(bind(&MOAIWebSocket::on_open,self,::_1));
	self->mClient.set_message_handler(bind(&MOAIWebSocket::on_message,self,::_1,::_2));
	self->mClient.set_fail_handler( bind( &MOAIWebSocket::on_fail, self, ::_1 ) );
  self->mClient.set_close_handler( bind( &MOAIWebSocket::on_close, self, ::_1 ) );
	
	MOAIWebSocketMgr::Get().AddHandle(*self);

	cc8* webUrl = state.GetValue < cc8* >( 2, "" );
	
	try {
		websocketpp::lib::error_code err;
		client::connection_ptr conn = self->mClient.get_connection( webUrl, err );
		if ( err ) {
			state.Push ( -1 );
			state.Push ( err.message().c_str() );
			printf("WebSocket Error: %s", err.message().c_str());

		} else {
			if ( conn ) {
					printf("WebSocket: Connecting to %s\n", webUrl);
					self->mClient.connect( conn );
					state.Push ( 0 );
					state.Push ( "WebSocket: connected." );
			} else {
					state.Push ( -1 );
					state.Push ( "Unable to resolve address." );
			}
		}
	} catch ( const std::exception& ex ) {
			printf("WebSocket Error: %s", ex.what());
			state.Push ( -1 );
			state.Push ( ex.what() );
	} catch ( websocketpp::lib::error_code err ) {
			printf("WebSocket Error: %s", err.message().c_str());
			state.Push ( -1 );
			state.Push ( err.message().c_str() );
	} catch ( ... ) {
			printf("WebSocket Error: An unknown exception occurred.");
			state.Push ( -1 );
			state.Push ( "An unknown exception occurred." );
	}
		
	return 0;
}

/**	@name	_write
	@text	Write a string to websocket.
	@in		message (string)
	@out  none
*/

int MOAIWebSocket::_write ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebSocket, "US" )

	cc8* msg = state.GetValue < cc8* >( 2, "" );

	if ( msg == NULL ) {
			self->notifyError("Cannot send empty message.");
	} else {
			websocketpp::lib::error_code err;
			self->mClient.send( self->mHandle, msg, websocketpp::frame::opcode::TEXT, err );
			if ( err ) {
				self->notifyError(err.message().c_str());
			} else {
			}
	}
}

/**	@name	_setListener
	@text	Set the listener to the instance
	@in		event (number)
	@in		callback (function)
	@out  none
*/

int MOAIWebSocket::_setListener ( lua_State* L ) {

	MOAI_LUA_SETUP ( MOAIWebSocket, "UNF" )
	
	u32 idx = state.GetValue < u32 >( 2, TOTAL );
	
	if ( idx < TOTAL ) {
		self->mListeners [ idx ].SetStrongRef ( state, 3);
	}
	
	return 0;
}

void MOAIWebSocket::notifyError(const char *error) {
		MOAILuaRef& callback = this->mListeners [ ON_FAIL ];
		
		if(callback) {
			MOAIScopedLuaState state = callback.GetSelf ();
			lua_pushstring(state, error);
			state.DebugCall ( 1, 0 );
		}
}

//================================================================//
// Event Calls
//================================================================//

void MOAIWebSocket::on_open(websocketpp::connection_hdl hdl) {

		mHandle = hdl;
		std::string message = "connection ready";
		MOAILuaRef& callback = this->mListeners [ ON_CONNECT ];
		if(callback) {
			MOAIScopedLuaState state = callback.GetSelf ();
			lua_pushstring(state, message.c_str());
			state.DebugCall ( 1, 0 );
		}
}

void MOAIWebSocket::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {

		MOAILuaRef& callback = this->mListeners [ ON_MESSAGE ];
		
		if(callback) {
			MOAIScopedLuaState state = callback.GetSelf ();
			lua_pushstring(state, msg->get_payload().c_str());
			state.DebugCall ( 1, 0 );
		}
}

void MOAIWebSocket::on_fail( websocketpp::connection_hdl handle ) {

		notifyError("transfer failed");
		mHandle = handle;
}

void MOAIWebSocket::on_close( websocketpp::connection_hdl handle ) {

		MOAILuaRef& callback = this->mListeners [ ON_CLOSE ];

		if(callback) {
			MOAIScopedLuaState state = callback.GetSelf ();
			lua_pushstring(state, "connection closed");
			state.DebugCall ( 1, 0 );
		}
}

//================================================================//
// Public Calls
//================================================================//

void MOAIWebSocket::Process () {
	mClient.poll();
}

MOAIWebSocket::MOAIWebSocket () {
	
	// register all classes MOAIFoo derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( MOAIFooBase )
	RTTI_END
}

MOAIWebSocket::~MOAIWebSocket () {
}

void MOAIWebSocket::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "ON_CONNECT",	( u32 )ON_CONNECT );
	state.SetField ( -1, "ON_FAIL",			( u32 )ON_FAIL );
	state.SetField ( -1, "ON_MESSAGE",	( u32 )ON_MESSAGE );
	state.SetField ( -1, "ON_CLOSE",		( u32 )ON_CLOSE );

	luaL_Reg regTable [] = {
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

void MOAIWebSocket::RegisterLuaFuncs ( MOAILuaState& state ) {

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "start",	_start },
		{ "write",  _write },
		{ "setListener", _setListener },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

