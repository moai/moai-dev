// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-http-client/MOAIWebSocket.h>
#include <moai-http-client/MOAIWebSocketMgr.h>

//#include <chrono>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	classHello
	@text	Class (a.k.a. static) method. Prints the string 'MOAIFoo class foo!' to the console.

	@out	nil
*/
int MOAIWebSocket::_classHello ( lua_State* L ) {
	UNUSED ( L );
	
	printf ( "MOAIFoo class foo!\n" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	instanceHello
	@text	Prints the string 'MOAIFoo instance foo!' to the console.

	@out	nil
*/
int MOAIWebSocket::_instanceHello ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebSocket, "U" ) // this macro initializes the 'self' variable and type checks arguments
	
	printf ( "MOAIFoo instance foo!\n" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	instanceHello
	@text	Prints the string 'MOAIFoo instance foo!' to the console.

	@out	nil
*/
int MOAIWebSocket::_start ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebSocket, "USF" )
	
	self->mClient.clear_access_channels(websocketpp::log::alevel::all);
	self->mClient.clear_error_channels(websocketpp::log::elevel::all);
	
	self->mClient.init_asio();
	self->mClient.set_open_handler(bind(&MOAIWebSocket::on_open,self,::_1));
	self->mClient.set_message_handler(bind(&MOAIWebSocket::on_message,self,::_1,::_2));
	self->mClient.set_fail_handler( bind( &MOAIWebSocket::on_fail, self, ::_1 ) );
	
	MOAIWebSocketMgr::Get().AddHandle(*self);

	cc8* webUrl = state.GetValue < cc8* >( 2, "" );
	self->SetLocal ( state, 3, self->mOnCallback );
	
	try {
		websocketpp::lib::error_code err;
		client::connection_ptr conn = self->mClient.get_connection( webUrl, err );
		if ( err ) {
			//mSignalError( err.message() );
			printf("ERROR: %s", err.message().c_str());

		} else {
			if ( conn ) {
					printf("### Connecting to %s\n", webUrl);
					self->mClient.connect( conn );
			} else {
					//mSignalError( "Unable to resolve address." );
					printf("Unable to resolve address.");
			}
		}
	} catch ( const std::exception& ex ) {
			//mSignalError( ex.what() );
			printf("%s", ex.what());
	} catch ( websocketpp::lib::error_code err ) {
			printf("%s", err.message().c_str());
			//mSignalError( err.message() );
	} catch ( ... ) {
			printf("An unknown exception occurred.");
			//mSignalError( "An unknown exception occurred." );
	}
		
	return 0;
}

int MOAIWebSocket::_write ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebSocket, "U" )

	cc8* msg = state.GetValue < cc8* >( 1, "" );

	if ( msg == NULL ) {
			//mSignalError( "Cannot send empty message." );
	} else {
			websocketpp::lib::error_code err;
			self->mClient.send( self->mHandle, msg, websocketpp::frame::opcode::TEXT, err );
			if ( err ) {
							//mSignalError( err.message() );
			} else {
							//mSignalWrite();
			}
	}
}

void MOAIWebSocket::on_open(websocketpp::connection_hdl hdl) {
    // now it is safe to use the connection
    std::cout << "### connection ready" << std::endl;
		mHandle = hdl;
}

void MOAIWebSocket::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

		this->PushLocal ( state, this->mOnCallback );
		lua_pushstring(state, msg->get_payload().c_str());
		state.DebugCall ( 1, 0 );
}

void MOAIWebSocket::on_fail( websocketpp::connection_hdl handle ) {
        mHandle = handle;
        printf( "Transfer failed.\n\n" );
}


//================================================================//
// MOAIFoo
//================================================================//

void MOAIWebSocket::Process () {
	mClient.poll();
}

//----------------------------------------------------------------//
MOAIWebSocket::MOAIWebSocket () {
	
	// register all classes MOAIFoo derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( MOAIFooBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIWebSocket::~MOAIWebSocket () {
}

//----------------------------------------------------------------//
void MOAIWebSocket::RegisterLuaClass ( MOAILuaState& state ) {

	// call any initializers for base classes here:
	// MOAIFooBase::RegisterLuaClass ( state );

	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "classHello",		_classHello },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIWebSocket::RegisterLuaFuncs ( MOAILuaState& state ) {

	// call any initializers for base classes here:
	// MOAIFooBase::RegisterLuaFuncs ( state );

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "start",	_start },
		{ "write",  _write },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

