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
void MOAITestResults::Push ( lua_State* L ) {

	MOAILuaState state ( L );

	lua_newtable ( state );

	lua_newtable ( state );
	CommentsIt commentsIt = this->mComments.begin ();
	for ( u32 i = 0; commentsIt != this->mComments.end (); ++commentsIt, ++i ) {
		state.Push ( i + 1 );
		state.Push ( *commentsIt );
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "comments" );
	
	state.Push ( this->mPassed );
	lua_setfield ( state, -2, "passed" );
	
	if ( !this->mPassed ) {
		state.Push ( this->mErrorMsg );
		lua_setfield ( state, -2, "error" );
	}
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAITestMgr::_assert ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	bool check = state.GetValue ( 1, false );
	if ( !check ) {
		state.CopyToTop ( 2 ); // message
		lua_error ( state );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_comment ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "S" )
	
	assert ( self->mCurrentTestResults );
	self->mCurrentTestResults->mComments.push_back ( state.GetValue < cc8* >( 1, "" ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_handleError ( lua_State* L ) {
	MOAILuaState state ( L );
	
	STLString msg;
	if ( lua_isstring ( L, 1 )) {  // 'message' a string?
		msg.write ( "%s\n", state.GetValue < cc8* >( 1, "" ));
	}
	msg.append ( state.GetStackTrace ( 0, 2 ));
	
	state.Push ( msg );
	return 1;
}

//----------------------------------------------------------------//
int MOAITestMgr::_results ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->PushResultsList ( L );
	return 1;
}

//----------------------------------------------------------------//
int MOAITestMgr::_runTests ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->RunTests ();
	return 0;
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
int MOAITestMgr::_test ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "SF-" )

	MOAITestResults& testResults = self->mTestResultsList.extend_back ();
	self->mCurrentTestResults = &testResults;

	lua_pushcfunction ( state, MOAITestMgr::_handleError );

	testResults.mTestName = state.GetValue < cc8* >( 1, "" );
	state.CopyToTop ( 2 );
	testResults.mPassed = lua_pcall ( state, 0, 0, 3 ) == 0;

	if ( !testResults.mPassed ) {

		cc8* msg = state.GetValue < cc8* >( -1, "" );
		testResults.mErrorMsg = msg;
		
		ZLLog::LogF ( ZLLog::CONSOLE, "%s - TEST FAILED\n%s\n", testResults.mTestName.c_str (), msg );
	}
	return 0;
}

//================================================================//
// MOAITestMgr
//================================================================//

//----------------------------------------------------------------//
MOAITestMgr::MOAITestMgr () :
	mCurrentTestResults ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITestMgr::~MOAITestMgr () {
}

//----------------------------------------------------------------//
void MOAITestMgr::PushResultsList ( lua_State* L ) {

	// results list
	lua_newtable ( L );

	TestResultsListIt testResultsListIt = this->mTestResultsList.begin ();
	for ( u32 i = 0; testResultsListIt != this->mTestResultsList.end (); ++testResultsListIt, ++i ) {
		MOAITestResults& testResults = *testResultsListIt;
		lua_pushnumber ( L, i + 1 );
		testResults.Push ( L );
		lua_settable ( L, -3 );
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "assert",					_assert },
		{ "comment",				_comment },
		{ "results",				_results },
		{ "runTests",				_runTests },
		{ "setProjectDir",			_setProjectDir },
		{ "setStagingDir",			_setStagingDir },
		{ "setStagingFunc",			_setStagingFunc },
		{ "setTestingDir",			_setTestingDir },
		{ "setTestingFunc",			_setTestingFunc },
		{ "test",					_test },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITestMgr::RunTests () {

	if ( !ZLFileSys::CheckPathExists ( this->mProjectDir )) return;

	ZLFileSys::AffirmPath ( this->mStagingDir );
	ZLFileSys::AffirmPath ( this->mTestingDir );

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	STLString oldPath = ZLFileSys::GetCurrentPath ();

	ZLFileSys::SetCurrentPath ( this->mProjectDir );

	ZLDirectoryItr dirItr;
	dirItr.Start ();
	
	while ( dirItr.NextDirectory ()) {
		
		this->mTestResultsList.clear ();
		
		STLString projectPath = this->mProjectDir + dirItr.Current ();
		STLString stagingPath = this->mStagingDir + dirItr.Current ();
		STLString testingPath = this->mTestingDir + dirItr.Current ();
		
		if ( !ZLFileSys::CheckPathExists ( stagingPath )) {
		
			ZLFileSys::Copy ( projectPath, stagingPath );
		
			// execute staging
			if ( this->mStagingFunc.PushRef ( state )) {
				ZLFileSys::SetCurrentPath ( stagingPath );
				state.DebugCall ( 0, 0 );
			}
		}

		ZLFileSys::DeleteDirectory ( testingPath, true );
		ZLFileSys::Copy ( stagingPath, testingPath );

		// execute test
		if ( this->mTestingFunc.PushRef ( state )) {
			ZLFileSys::SetCurrentPath ( testingPath );
			state.DebugCall ( 0, 0 );
		}
	}
	
	ZLFileSys::SetCurrentPath ( oldPath );
}
