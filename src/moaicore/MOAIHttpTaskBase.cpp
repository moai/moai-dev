// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <tinyxml.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIHttpTaskBase.h>
#include <moaicore/MOAIXmlParser.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getResponseCode
	@text	Returns the response code returned by the server after a httpPost or httpGet call.
 
	@in		MOAIHttpTask self
	@out	number code			The numeric response code returned by the server.
 */
int MOAIHttpTaskBase::_getResponseCode ( lua_State* L ) {
  MOAI_LUA_SETUP ( MOAIHttpTaskBase, "U" )
  	
  lua_pushnumber ( state, self->mResponseCode );
  
  return 1;
}

//----------------------------------------------------------------//
/**	@name	getResponseHeader
	@text	Returns the response header given its name, or nil if it wasn't provided by the server.
			Header names are case-insensitive and if multiple responses are given, they will be
			concatenated with a comma separating the values.
			
	@in		MOAIHttpTask self
	@in		string header			The name of the header to return (case-insensitive).
	@out	string response			The response given by the server or nil if none was specified.
*/
int MOAIHttpTaskBase::_getResponseHeader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "US" )

	STLString header ( state.GetValue < cc8* > ( 2, "" ));
	HeaderMap::iterator it = self->mResponseHeaders.find(header);

	if( it != self->mResponseHeaders.end () ) {
		lua_pushlstring ( state, it->second.c_str (), it->second.length ());
	}
	else {
		lua_pushnil ( state );
	}

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getSize
	@text	Returns the size of the string obtained from a httpPost or httpGet call.

	@in		MOAIHttpTaskBase self
	@out	number size				The string size. If the call found nothing, this will return the value zero (not nil).
*/
int MOAIHttpTaskBase::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "U" )

	lua_pushnumber ( state, self->mData.Size ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getString
	@text	Returns the text obtained from a httpPost or httpGet call.

	@in		MOAIHttpTaskBase self
	@out	string text				The text string.
*/
int MOAIHttpTaskBase::_getString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "U" )

	if ( self->mData.Size ()) {
		lua_pushlstring ( state, ( cc8* )self->mData.Data (), self->mData.Size ());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	httpGet
	@text	Sends an API call to the server for downloading data.  The callback function (from setCallback) will run when the call is complete, i.e. this action is asynchronous and returns almost instantly.

	@in		MOAIHttpTaskBase self
	@in		string url				The URL on which to perform the GET request.
	@opt	string useragent		Default value is "Moai SDK beta; support@getmoai.com"
	@opt	boolean verbose
	@opt	boolean blocking		Synchronous operation; block execution until complete. Default value is false.
	@out	nil
*/
int MOAIHttpTaskBase::_httpGet ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "US" )
	
	cc8* url		= state.GetValue < cc8* >( 2, "" );
	cc8* useragent	= state.GetValue < cc8* >( 3, DEFAULT_MOAI_HTTP_USERAGENT );
	bool verbose	= state.GetValue < bool >( 4, false );
	bool blocking	= state.GetValue < bool >( 5, false );
	
	self->InitForGet ( url, useragent, verbose );
	
	if ( blocking ) {
		self->PerformSync ();
	}
	else {
		self->PerformAsync ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	httpPost
	@text	Sends an API call to the server for downloading data.  The callback function (from setCallback) will run when the call is complete, i.e. this action is asynchronous and returns almost instantly.

	@overload

		@in		MOAIHttpTaskBase self
		@in		string url				The URL on which to perform the GET request.
		@opt	string data				The string containing text to send as POST data.
		@opt	string useragent		Default value is "Moai SDK beta; support@getmoai.com"
		@opt	boolean verbose
		@opt	boolean blocking		Synchronous operation; block execution until complete. Default value is false.
		@out	nil
	
	@overload

		@in		MOAIHttpTaskBase self
		@in		string url				The URL on which to perform the GET request.
		@opt	MOAIDataBuffer data		A MOAIDataBuffer object to send as POST data.
		@opt	string useragent
		@opt	boolean verbose
		@opt	boolean blocking		Synchronous operation; block execution until complete. Default value is false.
		@out	nil
*/
int MOAIHttpTaskBase::_httpPost ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "US" )
	
	cc8* url		= state.GetValue < cc8* >( 2, "" );
	cc8* useragent	= state.GetValue < cc8* >( 4, DEFAULT_MOAI_HTTP_USERAGENT );
	bool verbose	= state.GetValue < bool >( 5, false );
	bool blocking	= state.GetValue < bool >( 6, false );

	if ( state.IsType (3, LUA_TUSERDATA) ) {
		
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 3 );
		
		if ( data ) {
			
			void* bytes;
			size_t size;
			data->Lock ( &bytes, &size );
			self->InitForPost ( url, useragent, bytes, size, verbose );
			data->Unlock ();
		}
	}
	else if ( state.IsType (3, LUA_TSTRING )) {
		
		size_t size;
		cc8* postString = lua_tolstring ( state, 3, &size );
		self->InitForPost ( url, useragent, postString, size, verbose );
	}

	if ( blocking ) {
		self->PerformSync ();
	}
	else {
		self->PerformAsync ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	parseXml
	@text	Parses the text data returned from a httpGet or httpPost operation as XML and then returns a MOAIXmlParser with the XML content initialized.

	@in		MOAIHttpTaskBase self
	@out	MOAIXmlParser parser	The MOAIXmlParser which has parsed the returned data.
*/
int MOAIHttpTaskBase::_parseXml ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "U" )

	if ( !self->mData.Size ()) return 0;
	
	cc8* xml = ( cc8* )self->mData.Data ();
	
	TiXmlDocument doc;
	doc.Parse ( xml );
	MOAIXmlParser::Parse ( state, doc.RootElement ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name	performAsync
	@text	Perform the http task asynchronously.

	@in		MOAIHttpTaskBase self
	@out	nil
*/
int MOAIHttpTaskBase::_performAsync ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "U" )
	
	self->PerformAsync ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	performSync
	@text	Perform the http task synchronously ( blocking).

	@in		MOAIHttpTaskBase self
	@out	nil
*/
int MOAIHttpTaskBase::_performSync ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "U" )
	
	self->PerformSync ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setBody
	@text	Sets the body for a POST or PUT.

	@overload

		@in		MOAIHttpTaskBase self
		@opt	string data				The string containing text to send as POST data.
		@out	nil
	
	@overload

		@in		MOAIHttpTaskBase self
		@opt	MOAIDataBuffer data		A MOAIDataBuffer object to send as POST data.
		@out	nil
*/
int MOAIHttpTaskBase::_setBody ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "U" )

	if ( state.IsType (2, LUA_TUSERDATA) ) {
		
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2 );
		
		if ( data ) {
			
			void* bytes;
			size_t size;
			data->Lock ( &bytes, &size );
			self->SetBody ( bytes, size );
			data->Unlock ();
		}
	}
	else if ( state.IsType (2, LUA_TSTRING )) {
		
		size_t size;
		cc8* postString = lua_tolstring ( state, 2, &size );
		self->SetBody ( postString, size );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets the callback function used when a request is complete.

	@in		MOAIHttpTaskBase self
	@in		function callback		The function to execute when the HTTP request is complete.  The MOAIHttpTaskBase is passed as the first argument.
	@out	nil
*/
int MOAIHttpTaskBase::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "UF" )

	self->SetLocal ( state, 2, self->mOnFinish );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setHeader
	@text	Sets a custom header field. May be used to override default headers.

	@in		MOAIHttpTaskBase self
	@in		string key
	@opt	string value		Default is "".
	@out	nil
*/
int MOAIHttpTaskBase::_setHeader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "US" )

	cc8* key	= state.GetValue < cc8* >( 2, "" );
	cc8* value	= state.GetValue < cc8* >( 3, "" );

	self->SetHeader ( key, value );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUrl
	@text	Sets the URL for the task.

	@in		MOAIHttpTaskBase self
	@in		string url
	@out	nil
*/
int  MOAIHttpTaskBase::_setUrl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "US" )
	cc8* url = state.GetValue < cc8* >( 2, "" );
	self->SetUrl ( url );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUserAgent
	@text	Sets the 'useragent' header for the task.

	@in		MOAIHttpTaskBase self
	@opt	string useragent		Default value is "Moai SDK beta; support@getmoai.com"
	@out	nil
*/
int  MOAIHttpTaskBase::_setUserAgent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "U" )
	cc8* useragent = state.GetValue < cc8* >( 2, DEFAULT_MOAI_HTTP_USERAGENT );
	self->SetUserAgent ( useragent );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVerb
	@text	Sets the http verb.

	@in		MOAIHttpTaskBase self
	@in		number verb		One of MOAIHttpTaskBase.HTTP_GET, MOAIHttpTaskBase.HTTP_HEAD,
							MOAIHttpTaskBase.HTTP_POST, MOAIHttpTaskBase.HTTP_PUT,
							MOAIHttpTaskBase.HTTP_DELETE
	@out	nil
*/
int  MOAIHttpTaskBase::_setVerb ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "UN" )
	u32 verb = state.GetValue < u32 >( 2, 0 );
	self->SetVerb ( verb );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVerbose
	@text	Sets the task implementation to print out debug information (if any).

	@in		MOAIHttpTaskBase self
	@opt	boolean verbose		Default value is false.
	@out	nil
*/
int  MOAIHttpTaskBase::_setVerbose ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTaskBase, "U" )
	bool verbose = state.GetValue < bool >( 2, false );
	self->SetVerbose ( verbose );
	return 0;
}

//================================================================//
// MOAIHttpTaskBase
//================================================================//

//----------------------------------------------------------------//
void MOAIHttpTaskBase::Finish () {

	if ( this->mOnFinish ) {
		
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, this->mOnFinish );
		this->PushLuaUserdata ( state );
		state.Push ( this->GetResponseCode ());
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIHttpTaskBase::GetData ( void* buffer, u32 size ) {

	USByteStream byteStream;
	
	byteStream.SetBuffer ( this->mData, this->mData.Size ());
	byteStream.SetLength ( this->mData.Size ());
	byteStream.ReadBytes ( buffer, size );
}

//----------------------------------------------------------------//
void MOAIHttpTaskBase::InitForGet ( cc8* url, cc8* useragent, bool verbose ) {

	this->Reset ();
	
	this->SetUrl ( url );
	this->SetVerb ( HTTP_GET );
	this->SetUserAgent ( useragent );
	this->SetVerbose ( verbose );
}

//----------------------------------------------------------------//
void MOAIHttpTaskBase::InitForPost ( cc8* url, cc8* useragent, const void* buffer, u32 size, bool verbose ) {

	this->Reset ();
	
	this->SetUrl ( url );
	this->SetVerb ( HTTP_POST );
	this->SetBody ( buffer, size );
	this->SetUserAgent ( useragent );
	this->SetVerbose ( verbose );
}

//----------------------------------------------------------------//
MOAIHttpTaskBase::MOAIHttpTaskBase () :
	mResponseCode ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIHttpTaskBase::~MOAIHttpTaskBase () {
}

//----------------------------------------------------------------//
void MOAIHttpTaskBase::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "HTTP_GET",( u32 )   HTTP_GET );
	state.SetField ( -1, "HTTP_HEAD",( u32 )  HTTP_HEAD );
	state.SetField ( -1, "HTTP_POST",( u32 )  HTTP_POST );
	state.SetField ( -1, "HTTP_PUT",( u32 )	  HTTP_PUT );
	state.SetField ( -1, "HTTP_DELETE",( u32 )HTTP_DELETE );
}

//----------------------------------------------------------------//
void MOAIHttpTaskBase::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getResponseCode",	_getResponseCode },
		{ "getResponseHeader ",	_getResponseHeader },
		{ "getSize",			_getSize },
		{ "getString",			_getString },
		{ "httpGet",			_httpGet },
		{ "httpPost",			_httpPost },
		{ "parseXml",			_parseXml },
		{ "performAsync",		_performAsync },
		{ "performSync",		_performSync },
		{ "setCallback",		_setCallback },
		{ "setBody",			_setBody },
		{ "setHeader",			_setHeader },
		{ "setUrl",				_setUrl },
		{ "setUserAgent",		_setUserAgent },
		{ "setVerb",			_setVerb },
		{ "setVerbose",			_setVerbose },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIHttpTaskBase::SetHeader ( cc8* key, cc8* value ) {

	this->mHeaderMap [ key ] = value;
}
