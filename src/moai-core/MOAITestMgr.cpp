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
int MOAITestMgr::_assert ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	bool check = state.GetValue ( 1, false );
	if ( !check ) {
		state.CopyToTop ( 2 );
		lua_error ( state );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_beginTest ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_comment ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_endTest ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	return 0;
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
int MOAITestMgr::_setStagingFunc ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	self->mStagingFunc.SetRef ( state, 1 );
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setStagingDir ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->mStagingDir = ZLFileSys::GetAbsoluteDirPath ( state.GetValue < cc8* >( 1, "" ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setTestingFunc ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )

	self->mTestingFunc.SetRef ( state, 1 );
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_setTestingDir ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->mTestingDir = ZLFileSys::GetAbsoluteDirPath ( state.GetValue < cc8* >( 1, "" ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITestMgr::_test ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAITestMgr, "" )
	
	self->Test ( L, 2 );
	return 0;
}

//================================================================//
// MOAITestMgr
//================================================================//

//----------------------------------------------------------------//
MOAITestMgr::MOAITestMgr () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITestMgr::~MOAITestMgr () {
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "assert",					_assert },
		{ "beginTest",				_beginTest },
		{ "comment",				_comment },
		{ "endTest",				_endTest },
		{ "runTests",				_runTests },
		{ "setProjectDir",			_setProjectDir },
		{ "setStagingFunc",			_setStagingFunc },
		{ "setStagingDir",			_setStagingDir },
		{ "setTestingFunc",			_setTestingFunc },
		{ "setTestingDir",			_setTestingDir },
		{ "test",				_test },
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

//----------------------------------------------------------------//
void MOAITestMgr::Test ( lua_State* L, int idx ) {

	MOAILuaState state ( L );
	idx = state.AbsIndex ( idx );
	
	state.CopyToTop ( idx );
	state.DebugCall ( 0, 0 );
}
