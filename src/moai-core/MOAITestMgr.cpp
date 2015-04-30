// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-core/MOAITestMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
void MOAITestResult::Push ( lua_State* L ) {

	MOAILuaState state ( L );

	lua_newtable ( state );

	state.Push ( this->mName );
	lua_setfield ( state, -2, "name" );

	state.Push ( this->mPassed );
	lua_setfield ( state, -2, "passed" );

	if ( !this->mPassed ) {
		state.Push ( this->mErrorMsg );
		lua_setfield ( state, -2, "error" );
	}

	lua_newtable ( state );
	CommentsIt commentsIt = this->mComments.begin ();
	for ( u32 i = 0; commentsIt != this->mComments.end (); ++commentsIt, ++i ) {
		state.Push ( i + 1 );
		state.Push ( *commentsIt );
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "comments" );
	
	lua_newtable ( L );
	ChildrenIt childrenIt = this->mChildren.begin ();
	for ( u32 i = 0; childrenIt != this->mChildren.end (); ++childrenIt, ++i ) {
		MOAITestResult& child = *childrenIt;
		lua_pushnumber ( L, i + 1 );
		child.Push ( L );
		lua_settable ( L, -3 );
	}
	lua_setfield ( L, -2, "children" );
}

//----------------------------------------------------------------//
json_t* MOAITestResult::ToJson () {

	json_t* json = json_object ();

	json_object_set_new ( json, "name", json_string ( this->mName ));
	
	json_object_set ( json, "passed", this->mPassed ? json_true () : json_false ());

	if ( !this->mPassed ) {
		json_object_set_new ( json, "error", json_string ( this->mErrorMsg ));
	}

	json_t* comments = json_array ();
	CommentsIt commentsIt = this->mComments.begin ();
	for ( u32 i = 0; commentsIt != this->mComments.end (); ++commentsIt, ++i ) {
		json_array_append_new ( comments, json_string ( *commentsIt ));
	}
	json_object_set_new ( json, "comments", comments );
	
	json_t* children = json_array ();
	ChildrenIt childrenIt = this->mChildren.begin ();
	for ( u32 i = 0; childrenIt != this->mChildren.end (); ++childrenIt, ++i ) {
		MOAITestResult& child = *childrenIt;
		json_array_append_new ( children, child.ToJson ());
	}
	json_object_set_new ( json, "children", children );
	
	return json;
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
#ifndef _assert
int MOAITestMgr::_assert ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	if ( !state.IsTrueOrNotNil ( 1 )) {
		state.CopyToTop ( 2 ); // message
		lua_error ( state );
	}
	return 0;
}
#endif

//----------------------------------------------------------------//
int MOAITestMgr::_comment ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "S" )
	
	self->Comment ( state.GetValue < cc8* >( 1, "" ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_error ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "SC" )

	STLString msg;
	msg.write ( "%s\n", state.GetValue < cc8* >( 1, "" ));
	
	int depth = 0;
	lua_Debug ar;
	lua_State* co = lua_tothread ( state, 2 );
	while ( lua_getstack ( co, depth++, &ar )) {
		lua_getinfo ( co, "Snl", &ar );
		if ( ar.currentline > 0 ) {
			msg.write ( " - line %d in %s:%s ()\n", ar.currentline,  ar.short_src, ar.name ? ar.name : "<unknown>" );
		}
	}
	
	self->Error ( msg );
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_popTest ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->PopTest ();
	
	state.Push ( self->mCurrent ? true : false );
	return 1;
}

//----------------------------------------------------------------//
int MOAITestMgr::_pushTest ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	cc8* name = state.GetValue < cc8* >( 1, "" );
	self->PushTest ( name );
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_results ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->PushResults ( L );
	return 1;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setStepFunc ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	self->mStepFunc.SetRef ( state, 1 );
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setTimeout ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	self->SetTimeout ( state.GetValue < float >( 1, 0.0f ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_standalone ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	self->mStandalone = true;
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_suite ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	if ( self->mRoot ) {
		delete self->mRoot;
		self->mRoot = 0;
		self->mCurrent = 0;
	}
	self->mSuiteName = state.GetValue < cc8* >( 1, ZLFileSys::GetCurrentPath ());
	return 0;
}

//================================================================//
// MOAITestMgr
//================================================================//

//----------------------------------------------------------------//
void MOAITestMgr::Abort () {

	while ( this->mCurrent ) {
		this->PopTest ();
	}
	//if ( this->mStandalone ) {
		exit ( 0 );
	//}
}

//----------------------------------------------------------------//
void MOAITestMgr::Comment ( cc8* msg ) {

	if ( msg && this->mCurrent ) {
		this->mCurrent->mComments.push_back ( msg );
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::Error ( cc8* msg ) {

	if ( msg && this->mCurrent ) {
	
		MOAITestResult& testResult = * this->mCurrent;
	
		testResult.mPassed = false;
		testResult.mErrorMsg = msg;
		
		ZLLog::LogF ( ZLLog::CONSOLE, "%s - FAILED\n%s\n", testResult.mName.c_str (), testResult.mErrorMsg.c_str ());
	}
}

//----------------------------------------------------------------//
MOAITestMgr::MOAITestMgr () :
	mRoot ( 0 ),
	mCurrent ( 0 ),
	mStandalone ( false ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITestMgr::~MOAITestMgr () {
}

//----------------------------------------------------------------//
void MOAITestMgr::OnAlarm ( int signum ) {
	UNUSED ( signum );
	
	MOAITestMgr& testMgr = MOAITestMgr::Get ();
	testMgr.Error ( "TIME LIMIT EXPIRED" );
	testMgr.Abort ();
}

//----------------------------------------------------------------//
void MOAITestMgr::PopTest () {
	
	this->SetTimeout (); // clear the timeout, if any
	
	if ( this->mCurrent  ) {
	
		MOAITestResult& testResult = *this->mCurrent;
	
		if (( testResult.mPassed == false ) && testResult.mParent ) {
			testResult.mParent->mPassed = false;
		}
		this->mCurrent = testResult.mParent;
	}
	
	if ( !this->mCurrent ) {
		this->WriteLog ();
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::PushResults ( lua_State* L ) {

	if ( this->mRoot ) {
		this->mRoot->Push ( L );
	}
	else {
		lua_pushnil ( L );
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::PushTest ( cc8* name ) {

	MOAITestResult* parent = this->mCurrent;

	if ( !parent ) {
		if ( this->mRoot ) {
			delete this->mRoot;
		}
		this->mRoot = new MOAITestResult ();
		this->mRoot->mName = this->mSuiteName;
	}

	this->mCurrent = parent ? &parent->mChildren.extend_back () : this->mRoot;
	this->mCurrent->mParent = parent;
	this->mCurrent->mPassed = true;
	this->mCurrent->mName = name ? name : "";
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
#ifndef _assert
		{ "assert",					_assert },
#endif
		{ "comment",				_comment },
		{ "error",					_error },
		{ "popTest",				_popTest },
		{ "pushTest",				_pushTest },
		{ "results",				_results },
		{ "setStepFunc",			_setStepFunc },
		{ "setTimeout",				_setTimeout },
		{ "standalone",				_standalone },
		{ "suite",					_suite },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITestMgr::SetTimeout () {
#ifndef _MSC_VER
	struct sigaction sa;

	memset ( &sa, 0, sizeof ( sa ));
	sa.sa_handler = SIG_IGN;
	sigaction ( SIGALRM, &sa, NULL );
#endif
}

//----------------------------------------------------------------//
void MOAITestMgr::SetTimeout ( float seconds ) {
#ifndef _MSC_VER
	struct sigaction sa;

	memset ( &sa, 0, sizeof ( sa ));
	sa.sa_handler = MOAITestMgr::OnAlarm;
	sigaction ( SIGALRM, &sa, NULL );

	itimerval ival;

	memset ( &ival, 0, sizeof ( ival ));
	ival.it_value.tv_sec = ( int )seconds;
	ival.it_value.tv_usec = ( int )( ZLFloat::Decimal ( seconds ) * 1000000.f );

	setitimer ( ITIMER_REAL, &ival, 0 );
#endif
}

//----------------------------------------------------------------//
void MOAITestMgr::Step () {

	if ( this->mStepFunc ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		this->mStepFunc.PushRef ( state );
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::WriteLog () {

	if ( this->mRoot ) {
	
		json_t* json = this->mRoot->ToJson ();
		STLString out = json_dumps ( json, JSON_INDENT ( 4 ) | JSON_SORT_KEYS );
		json_decref ( json );
		out.append ( "\n" );
		
		ZLFileStream stream;
		stream.Open ( "log.json", ZLFileStream::READ_WRITE_NEW );
		stream.Print ( "%s", out.c_str ());
		stream.Close ();
	}
}
