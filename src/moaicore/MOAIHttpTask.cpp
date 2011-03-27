// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <tinyxml.h>
#include <moaicore/MOAIHttpTask.h>
#include <moaicore/MOAIXmlParser.h>
#include <moaicore/MOAIDataBuffer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>getSize ( )</tt>\n
\n
	Returns the size of the string obtained from an httpPost or httpGet call.
	@return String size.  If the call found nothing, this will return the value zero (not nil).
*/
int MOAIHttpTask::_getSize ( lua_State* L ) {
	LUA_SETUP ( MOAIHttpTask, "U" )

	lua_pushnumber ( state, self->mSize );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>getText ( )</tt>\n
\n
	Returns the text obtained from an httpPost or httpGet call.
	@return The text string.
*/
int MOAIHttpTask::_getString ( lua_State* L ) {
	LUA_SETUP ( MOAIHttpTask, "U" )

	lua_pushlstring ( state, ( cc8* )self->mBuffer, self->mSize );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>httpGet ( url )</tt>\n
\n
	Sends an API call to the server for downloading data.  The callback function (from setCallback) will run when the call is complete.
	@param url A string containing the full url of the API call.
*/
int MOAIHttpTask::_httpGet ( lua_State* L ) {
	LUA_SETUP ( MOAIHttpTask, "US" )
	
	cc8* url = lua_tostring ( state, 2 );
	
	USHttpTask* task = new USHttpTask ();
	task->SetDelegate < MOAIHttpTask >( self, &MOAIHttpTask::OnHttpFinish );
	task->HttpGet ( url );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>httpPost ( url, string|data )</tt>\n
\n
	Posts an API call to the server for uploading data.  The callback function (from setCallback) will run when the call is complete.
	@param url A string containing the full url of the API call.
	@param string A character string containing text to send as POST data.
	@param data A MOAIDataBuffer object to send as POST data.
*/
int MOAIHttpTask::_httpPost ( lua_State* L ) {
	LUA_SETUP ( MOAIHttpTask, "US" )
	
	cc8* url = lua_tostring ( state, 2 );

	if ( state.IsType (3, LUA_TUSERDATA) ) {
		
		self->mPostData = state.GetLuaData < MOAIDataBuffer >( 3 );
		
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
/**	@brief <tt>parseXml ( url )</tt>\n
\n
	Parses an XML table obtained from httpGet.
	@return Parsed XML code.
*/
int MOAIHttpTask::_parseXml ( lua_State* L ) {
	LUA_SETUP ( MOAIHttpTask, "U" )

	if ( !self->mSize ) return 0;
	
	cc8* xml = ( cc8* )self->mBuffer;
	
	TiXmlDocument doc;
	doc.Parse ( xml );
	MOAIXmlParser::Parse ( state, doc.RootElement ());

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>setCallback ( callback )</tt>\n
\n
	Sets the function to be called when an httpGet or httpPost is completed.
	@param callback The function to be called.  The completed HTTPtask is passed into the function as the first argument.
*/
int MOAIHttpTask::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAIHttpTask, "UF" )

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
		this->mBuffer = 0;
		this->mSize = 0;
	}
}

//----------------------------------------------------------------//
MOAIHttpTask::MOAIHttpTask () :
	mBuffer ( 0 ),
	mSize ( 0 ) {

	RTTI_SINGLE ( USLuaData )
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
	
		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		
		this->mOnFinish.PushRef ( state );
		this->PushLuaInstance ( state );
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

	LuaReg regTable [] = {
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
