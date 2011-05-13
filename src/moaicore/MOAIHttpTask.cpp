// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <tinyxml.h>
#include <moaicore/MOAIHttpTask.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIXmlParser.h>
#include <moaicore/MOAIDataBuffer.h>

//================================================================//
// local
//================================================================//

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
		lua_pushlstring ( state, ( cc8* )self->mBuffer, self->mSize );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	httpGet
	@text	Sends an API call to the server for downloading data.  The callback function (from setCallback) will run when the call is complete, i.e. this action is asynchronous and returns almost instantly.

	@in		MOAIHttpTask self
	@in		string url				The URL on which to perform the GET request.
	@out	nil
*/
int MOAIHttpTask::_httpGet ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "US" )
	
	cc8* url = lua_tostring ( state, 2 );
	
	USHttpTask* task = new USHttpTask ();
	task->SetDelegate < MOAIHttpTask >( self, &MOAIHttpTask::OnHttpFinish );
	task->HttpGet ( url );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	httpPost
	@text	Sends an API call to the server for downloading data.  The callback function (from setCallback) will run when the call is complete, i.e. this action is asynchronous and returns almost instantly.

	@in		MOAIHttpTask self
	@in		string url				The URL on which to perform the GET request.
	@opt	string data				The string containing text to send as POST data.
	@opt	MOAIDataBuffer data		A MOAIDataBuffer object to send as POST data.  You must either provide a MOAIDataBuffer or a string for data, but not both.
	@out	nil
*/
int MOAIHttpTask::_httpPost ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHttpTask, "US" )
	
	cc8* url = lua_tostring ( state, 2 );

	if ( state.IsType (3, LUA_TUSERDATA) ) {
		
		self->mPostData = state.GetLuaObject < MOAIDataBuffer >( 3 );
		
		void* bytes;
		u32 size;
		self->mPostData->Lock ( &bytes, &size );
		
		USHttpTask* task = new USHttpTask ();
		task->SetDelegate < MOAIHttpTask >( self, &MOAIHttpTask::OnHttpFinish );
		task->HttpPost ( url, bytes, size );
		
		self->mPostData->Unlock ();
	}
	else {
		
		if ( !state.IsType (3, LUA_TSTRING ) ) return 0;
		
		self->mPostString = lua_tostring ( state, 3 );
		
		USHttpTask* task = new USHttpTask ();
		task->SetDelegate < MOAIHttpTask >( self, &MOAIHttpTask::OnHttpFinish );
		task->HttpPost ( url, self->mPostString.str (), ( u32 )self->mPostString.size ());
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
	
	cc8* xml = ( cc8* )self->mBuffer;
	
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

	self->mOnFinish.SetRef ( state, 2, false );

	return 0;
}

//================================================================//
// MOAIHttpTask
//================================================================//

//----------------------------------------------------------------//
void MOAIHttpTask::Clear () {

	if ( this->mBuffer ) {
		free ( this->mBuffer );
	}
	this->mBuffer = 0;
	this->mSize = 0;
}

//----------------------------------------------------------------//
MOAIHttpTask::MOAIHttpTask () :
	mBuffer ( 0 ),
	mSize ( 0 ) {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIHttpTask::~MOAIHttpTask () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTask::Init ( u32 size ) {

	this->Clear ();
	this->mSize = size;
	this->mBuffer = malloc ( size + 1 );
	
	u8* buffer = ( u8* )this->mBuffer;
	buffer [ size ] = 0;
}

//----------------------------------------------------------------//
void MOAIHttpTask::OnHttpFinish ( USHttpTask* task ) {

	this->Clear ();
	
	u32 size = task->GetSize ();
	if ( size ) {
	
		this->Init ( size );
		task->GetData ( this->mBuffer, this->mSize );
	}
	
	if ( this->mOnFinish ) {
	
		USLuaStateHandle state = this->mOnFinish.GetSelf ();
		this->PushLuaUserdata ( state );
		state.DebugCall ( 1, 0 );
		
	}
	
	this->mPostData = 0;
	this->mPostString.clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTask::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIHttpTask::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
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

//----------------------------------------------------------------//
STLString MOAIHttpTask::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mBuffer )
	PRETTY_PRINT ( repr, mSize )

	return repr;
}
