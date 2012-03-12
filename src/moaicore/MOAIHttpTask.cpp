// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <tinyxml.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIHttpTask.h>
#include <moaicore/MOAIUrlMgr.h>
#include <moaicore/MOAIXmlParser.h>

#ifdef _WIN32
	#include <algorithm>
#endif

#ifdef MOAI_OS_NACL
	#include <moaicore/MOAIHttpTaskInfo_nacl.h>
#else
	#include <moaicore/MOAIHttpTaskInfo_curl.h>
#endif

//================================================================//
// local
//================================================================//


//----------------------------------------------------------------//
/**	@name	getResponseCode
 @text	Returns the response code returned by the server after a httpPost or httpGet call.
 
 @in		MOAIHttpTask self
 @out	number code					The numeric response code returned by the server.
 */
int MOAIHttpTask::_getResponseCode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "U" )
	
	lua_pushnumber ( state, self->GetInfo()->mResponseCode );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getResponseHeader
 @text	Returns the response header given its name, or nil if it wasn't provided by the server.
		Header names are case-insensitive and if multiple responses are given, they will be
		concatenated with a comma separating the values.
 @in		MOAIHttpTask self
 @in	string header	The name of the header to return (case-insensitive).
 @out	string response				The response given by the server or nil if none was specified.
*/
int MOAIHttpTask::_getResponseHeader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "US" )
	
	STLString header(state.GetValue < cc8* >( 2, "" ));
	std::transform(header.begin(), header.end(), header.begin(), ::tolower);
	MOAIHttpTaskInfo *info = self->GetInfo();
	MOAIHttpTaskInfo::HeaderMap::iterator it = info->mResponseHeaders.find(header);
	if( it != info->mResponseHeaders.end() )
	{
		lua_pushlstring ( state, it->second.c_str(), it->second.length() );
	}
	else
	{
		lua_pushnil( state );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getSize
	@text	Returns the size of the string obtained from a httpPost or httpGet call.

	@in		MOAIHttpTask self
	@out	number size				The string size.  If the call found nothing, this will return the value zero (not nil).
*/
int MOAIHttpTask::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "U" )

	lua_pushnumber ( state, self->mSize );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getString
	@text	Returns the text obtained from a httpPost or httpGet call.

	@in		MOAIHttpTask self
	@out	string text				The text string.
*/
int MOAIHttpTask::_getString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "U" )

	if ( self->mSize ) {
		lua_pushlstring ( state, ( cc8* )self->mData, self->mSize );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	httpGet
	@text	Sends an API call to the server for downloading data.  The callback function (from setCallback) will run when the call is complete, i.e. this action is asynchronous and returns almost instantly.

	@in		MOAIHttpTask self
	@in		string url				The URL on which to perform the GET request.
	@opt	string useragent		Default value is "Moai SDK beta; support@getmoai.com"
	@opt	boolean verbose
	@opt	boolean blocking		Synchronous operation; block execution until complete. Default value is false.
	@out	nil
*/
int MOAIHttpTask::_httpGet ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "US" )
	
	
	cc8* url		= state.GetValue < cc8* >( 2, "" );
	cc8* useragent	= state.GetValue < cc8* >( 3, DEFAULT_MOAI_HTTP_USERAGENT );
	bool verbose	= state.GetValue < bool >( 4, false );
	bool blocking	= state.GetValue < bool >( 5, false );

	HeaderList headers;
	if( state.IsType(6, LUA_TTABLE) )
	{
		int itr = state.PushTableItr ( 6 );
		while ( state.TableItrNext ( itr )) {
			cc8* h = state.GetValue < cc8* >( -1, 0 );
			headers.push_back(h);
		}
	}

	self->HttpGet ( url, useragent, verbose, blocking, &headers );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	httpPost
	@text	Sends an API call to the server for downloading data.  The callback function (from setCallback) will run when the call is complete, i.e. this action is asynchronous and returns almost instantly.

	@overload

		@in		MOAIHttpTask self
		@in		string url				The URL on which to perform the GET request.
		@opt	string data				The string containing text to send as POST data.
		@opt	string useragent		Default value is "Moai SDK beta; support@getmoai.com"
		@opt	boolean verbose
		@opt	boolean blocking		Synchronous operation; block execution until complete. Default value is false.
		@out	nil
	
	@overload

		@in		MOAIHttpTask self
		@in		string url				The URL on which to perform the GET request.
		@opt	MOAIDataBuffer data		A MOAIDataBuffer object to send as POST data.
		@opt	string useragent
		@opt	boolean verbose
		@opt	boolean blocking		Synchronous operation; block execution until complete. Default value is false.
		@out	nil
*/
int MOAIHttpTask::_httpPost ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "US" )
	
	cc8* url		= state.GetValue < cc8* >( 2, "" );
	cc8* useragent	= state.GetValue < cc8* >( 4, DEFAULT_MOAI_HTTP_USERAGENT );
	bool verbose	= state.GetValue < bool >( 5, false );
	bool blocking	= state.GetValue < bool >( 6, false );
	
	HeaderList headers;
	if( state.IsType(7, LUA_TTABLE) )
	{
		int itr = state.PushTableItr ( 6 );
		while ( state.TableItrNext ( itr )) {
			cc8* h = state.GetValue < cc8* >( -1, 0 );
			headers.push_back(h);
		}
	}

	if ( state.IsType (3, LUA_TUSERDATA) ) {
		
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 3 );
		
		if ( data ) {
		
			void* bytes;
			size_t size;
			data->Lock ( &bytes, &size );
			self->HttpPost ( url, useragent, bytes, size, verbose, blocking, &headers );
			data->Unlock ();
		}
	}
	else if ( state.IsType (3, LUA_TSTRING )) {
		
		cc8* postString = lua_tostring ( state, 3 );
		self->HttpPost ( url, useragent, postString, ( u32 )strlen ( postString ), verbose, blocking, &headers );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	parseXml
	@text	Parses the text data returned from a httpGet or httpPost operation as XML and then returns a MOAIXmlParser with the XML content initialized.

	@in		MOAIHttpTask self
	@out	MOAIXmlParser parser	The MOAIXmlParser which has parsed the returned data.
*/
int MOAIHttpTask::_parseXml ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "U" )

	if ( !self->mSize ) return 0;
	
	cc8* xml = ( cc8* )self->mData;
	
	TiXmlDocument doc;
	doc.Parse ( xml );
	MOAIXmlParser::Parse ( state, doc.RootElement ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets the callback function used when a request is complete.

	@in		MOAIHttpTask self
	@in		function callback		The function to execute when the HTTP request is complete.  The MOAIHttpTask is passed as the first argument.
	@out	nil
*/
int MOAIHttpTask::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "UF" )

	self->SetLocal ( state, 2, self->mOnFinish );
	return 0;
}

//================================================================//
// MOAIHttpTask
//================================================================//

//----------------------------------------------------------------//
void MOAIHttpTask::Clear () {

	if ( this->mInfo ) {
		delete this->mInfo;
		this->mInfo = 0;
	}
	
	this->mData = 0;
	this->mSize = 0;
}

//----------------------------------------------------------------//
void MOAIHttpTask::Finish () {

	this->mInfo->Finish ();
	
	this->mData = this->mInfo->mData;
	this->mSize = this->mInfo->mData.Size ();
	this->mResponseCode = this->mInfo->mResponseCode;

	if ( this->mOnFinish ) {
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, this->mOnFinish );
		this->PushLuaUserdata ( state );
		state.Push ( this->GetResponseCode ());
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIHttpTask::GetData ( void* buffer, u32 size ) {

	USByteStream byteStream;
	
	byteStream.SetBuffer ( this->mData, this->mSize );
	byteStream.SetLength ( this->mSize );
	byteStream.ReadBytes ( buffer, size );
}

//----------------------------------------------------------------//
void MOAIHttpTask::HttpGet ( cc8* url, cc8* useragent, bool verbose, bool blocking, HeaderList *extraHeaders ) {

	this->Clear ();
	
	this->mInfo = new MOAIHttpTaskInfo ();
	this->mInfo->InitForGet ( url, useragent, verbose, extraHeaders );
	
	if ( blocking ) {
		this->mInfo->PerformSync ();
		this->Finish ();
	}
	else {
		MOAIUrlMgr::Get ().AddHandle ( *this );
	}
}

//----------------------------------------------------------------//
void MOAIHttpTask::HttpPost ( cc8* url, cc8* useragent, const void* buffer, u32 size, bool verbose, bool blocking, HeaderList *extraHeaders ) {

	this->Clear ();
	
	this->mInfo = new MOAIHttpTaskInfo ();
	this->mInfo->InitForPost ( url, useragent, buffer, size, verbose, extraHeaders );
	
	if ( blocking ) {
		this->mInfo->PerformSync ();
		this->Finish ();
	}
	else {
		MOAIUrlMgr::Get ().AddHandle ( *this );
	}
}

//----------------------------------------------------------------//
MOAIHttpTask::MOAIHttpTask () :
	mInfo ( 0 ),
	mData ( 0 ),
	mSize ( 0 ),
	mVerbose ( false ),
	mResponseCode ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIHttpTask::~MOAIHttpTask () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTask::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIHttpTask::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getResponseCode",	_getResponseCode },
		{ "getResponseHeader",	_getResponseHeader },
		{ "getSize",			_getSize },
		{ "getString",			_getString },
		{ "httpGet",			_httpGet },
		{ "httpPost",			_httpPost },
		{ "parseXml",			_parseXml },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}