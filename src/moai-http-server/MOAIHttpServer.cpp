// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-http-server/MOAIHttpServer.h>

#define MAX_OPTIONS 100

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIHttpServer::_start ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpServer, "U" )
	
	cc8* options [ MAX_OPTIONS ];
	u32 top = 0;
	
	if ( state.IsType ( 2, LUA_TTABLE )) {
		u32 itr = state.PushTableItr ( 2 );
		while ( state.TableItrNext ( itr )) {
			options [ top++ ] = state.GetValue < cc8* >( -2, "" );
			options [ top++ ] = state.GetValue < cc8* >( -1, "" );
		}
		state.Pop ( 1 );
	}
	
	options [ top ] = 0;
	self->Start ( options );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIHttpServer::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpServer, "U" )
	
	self->Stop ();
	
	return 0;
}

//================================================================//
// mongoose callbacks
//================================================================//

//----------------------------------------------------------------//
int MOAIHttpServer::_beginRequest ( mg_connection* connection ) {
	
	mg_request_info* info = mg_get_request_info ( connection );
	MOAIHttpServer* self = ( MOAIHttpServer* )info->user_data;

	return self->HandleRequest ( connection, info );
}

//----------------------------------------------------------------//
void MOAIHttpServer::_endRequest ( const mg_connection* connection, int replyStatusCode ) {
	UNUSED ( connection );
	UNUSED ( replyStatusCode );

	printf ( "\n" );
}

//----------------------------------------------------------------//
int MOAIHttpServer::_logMessage ( const mg_connection* connection, const char* message ) {
	UNUSED ( connection );
	UNUSED ( message );

	return 0;
}

//================================================================//
// MOAIHttpServer
//================================================================//

//----------------------------------------------------------------//
int MOAIHttpServer::HandleRequest ( mg_connection* connection, mg_request_info* info ) {

	if ( MOAILuaRuntime::IsValid ()) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->PushListenerAndSelf ( EVENT_HANDLE_REQUEST, state )) {

			// http://localhost:8080/foo/bar/baz?a=b&c=d
			
			//const char *request_method; // "GET", "POST", etc
			//const char *uri;            // URL-decoded URI
			//const char *http_version;   // E.g. "1.0", "1.1"
			//const char *query_string;   // URL part after '?', not including '?', or NULL
			//const char *remote_user;    // Authenticated user, or NULL if no auth used
			//long remote_ip;             // Client's IP address
			//int remote_port;            // Client's port
			//int is_ssl;                 // 1 if SSL-ed, 0 if not
			//void *user_data;            // User data pointer passed to mg_start()

			//int num_headers;            // Number of HTTP headers
			//struct mg_header {
			//	const char *name;         // HTTP header name
			//	const char *value;        // HTTP header value
			//} http_headers[64];         // Maximum 64 headers
			
			state.Push ( info->request_method );
			state.Push ( info->uri );
			state.Push ( info->query_string );
			
			if ( info->num_headers ) {
				lua_newtable ( state );
				for ( int i = 0; i < info->num_headers; ++i ) {
					state.SetField ( -1, info->http_headers [ i ].name, info->http_headers [ i ].value );
				}
			}
			
			state.DebugCall ( 5, 3 );
			
			if ( !state.IsType ( -3, LUA_TNIL )) {
			
				u32 statusCode = state.GetValue < u32 >( -3, 400 );
				
				cc8* contentType = state.GetValue < cc8* >( -2, "text/plain" );
				
				size_t contentLength = 0;
				cc8* content = lua_tolstring ( state, -1, &contentLength );
				
				if ( contentLength ) {
				
					// Send HTTP reply to the client
					mg_printf (
						connection,
						"HTTP/1.1 %d OK\r\n"
						"Content-Type: %s\r\n"
						"Content-Length: %d\r\n"		// Always set Content-Length
						"\r\n"
						"%s",
						statusCode,
						contentType,
						( int )contentLength,
						content
					);
					
					// Returning non-zero tells mongoose that our function has replied to
					// the client, and mongoose should not send client any more data.
					return 1;
				}
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIHttpServer::MOAIHttpServer () :
	mContext ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIInstanceEventSource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHttpServer::~MOAIHttpServer () {

	this->Stop ();
}

//----------------------------------------------------------------//
void MOAIHttpServer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaClass ( state );

	state.SetField ( -1, "EVENT_HANDLE_REQUEST", ( u32 )EVENT_HANDLE_REQUEST );
}

//----------------------------------------------------------------//
void MOAIHttpServer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "start",			_start },
		{ "stop",			_stop },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIHttpServer::Start ( cc8** options ) {

	this->Stop ();

	struct mg_callbacks callbacks;
	memset ( &callbacks, 0, sizeof ( callbacks ));
	callbacks.begin_request = _beginRequest;
	callbacks.end_request = _endRequest;
	callbacks.log_message = _logMessage;
	
	this->mContext = mg_start ( &callbacks, this, options );
	
	//printf (
	//	"%s started on port(s) %s with web root [%s]\n",
	//	server_name,
	//	mg_get_option ( ctx, "listening_ports" ),
	//	mg_get_option ( ctx, "document_root" )
	//);
}

//----------------------------------------------------------------//
void MOAIHttpServer::Stop () {

	if ( this->mContext ) {
		mg_stop ( this->mContext );
		this->mContext = 0;
	}
}
