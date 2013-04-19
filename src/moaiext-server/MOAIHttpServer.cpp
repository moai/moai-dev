// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-server/MOAIHttpServer.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIHttpServer::_start ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpServer, "U" )
	
	self->Start ( 0 );
	
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

	// http://localhost:8080/foo/bar/baz?a=blarg&b=zowie

	printf ( "//----------------------------------------------------------------//\n" );
	printf ( "begin_request\n" );
	
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
	
	mg_request_info* info = mg_get_request_info ( connection );
	MOAIHttpServer* self = ( MOAIHttpServer* )info->user_data;
	
	printf ( "method:  %s\n", info->request_method );
	printf ( "uri:     %s\n", info->uri );
	printf ( "version: %s\n", info->http_version );
	printf ( "query:   %s\n", info->query_string );
	printf ( "user:    %s\n", info->remote_user );
	
	char content [ 100 ];

	// Prepare the message we're going to send
	int content_length = _snprintf (
		content,
		sizeof ( content ),
		"Hello from mongoose! Remote port: %d",
		info->remote_port
	);

	// Send HTTP reply to the client
	mg_printf (
		connection,
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: %d\r\n"		// Always set Content-Length
		"\r\n"
		"%s",
		content_length,
		content
	);

	// Returning non-zero tells mongoose that our function has replied to
	// the client, and mongoose should not send client any more data.
	return 1;
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
MOAIHttpServer::MOAIHttpServer () :
	mContext ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHttpServer::~MOAIHttpServer () {

	this->Stop ();
}

//----------------------------------------------------------------//
void MOAIHttpServer::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIHttpServer::RegisterLuaFuncs ( MOAILuaState& state ) {

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
