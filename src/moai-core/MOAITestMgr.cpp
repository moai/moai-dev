// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-core/MOAITestMgr.h>

// run the test in each folder until the test ends OR TestMgr.finish () is called
// optionally, tear down and rebuild the context between each run
// run the test in each folder using a bootstrapper
// bootstrappers
//		run test or staging until done
//		run test or staging in new context until done
//		run test or staging in new process until done

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
	
	assert ( self->mCurrent );
	self->mCurrent->mComments.push_back ( state.GetValue < cc8* >( 1, "" ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_error ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "SC" )

	assert ( self->mCurrent );
	MOAITestResult& testResult = *self->mCurrent;

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
	
	testResult.mPassed = false;
	testResult.mErrorMsg = msg;
	
	ZLLog::LogF ( ZLLog::CONSOLE, "%s - FAILED\n%s\n", testResult.mName.c_str (), testResult.mErrorMsg.c_str ());

	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_pop_test ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	if ( self->mCurrent  ) {
	
		MOAITestResult& testResult = *self->mCurrent;
	
		if (( testResult.mPassed == false ) && testResult.mParent ) {
			testResult.mParent->mPassed = false;
		}
		self->mCurrent = testResult.mParent;
	}
	else {
		self->mCurrent = 0;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_push_test ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	MOAITestResult* parent = self->mCurrent;

	if ( !parent ) {
		if ( self->mRoot ) {
			delete self->mRoot;
		}
		self->mRoot = new MOAITestResult ();
		self->mRoot->mName = self->mSuiteName;
	}

	self->mCurrent = parent ? &parent->mChildren.extend_back () : self->mRoot;
	self->mCurrent->mParent = parent;
	self->mCurrent->mPassed = true;

	if ( state.IsType ( 1, LUA_TSTRING )) {
		self->mCurrent->mName = state.GetValue < cc8* >( 1, "" );
	}

	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_results ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->PushResults ( L );
	return 1;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setProjectDir ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->mProjectDir = ZLFileSys::GetAbsoluteDirPath ( state.GetValue < cc8* >( 1, "" ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setStagingDir ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->mStagingDir = ZLFileSys::GetAbsoluteDirPath ( state.GetValue < cc8* >( 1, "" ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setStagingFunc ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	self->mStagingFunc.SetRef ( state, 1 );
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setTestingDir ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->mTestingDir = ZLFileSys::GetAbsoluteDirPath ( state.GetValue < cc8* >( 1, "" ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setTestingFunc ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	self->mTestingFunc.SetRef ( state, 1 );
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
MOAITestMgr::MOAITestMgr () :
	mRoot ( 0 ),
	mCurrent ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITestMgr::~MOAITestMgr () {
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
void MOAITestMgr::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
#ifndef _assert
		{ "assert",					_assert },
#endif
		{ "comment",				_comment },
		{ "error",					_error },
		{ "pop_test",				_pop_test },
		{ "push_test",				_push_test },
		{ "results",				_results },
		{ "setProjectDir",			_setProjectDir },
		{ "setStagingDir",			_setStagingDir },
		{ "setStagingFunc",			_setStagingFunc },
		{ "setTestingDir",			_setTestingDir },
		{ "setTestingFunc",			_setTestingFunc },
		{ "suite",					_suite },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
