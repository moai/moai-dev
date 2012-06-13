// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-test/MOAITestMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITestMgr::_beginTest ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* testName = state.GetValue < cc8* >( 1, 0 );
	if ( testName ) {
		MOAITestMgr::Get ().BeginTest ( testName );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITestMgr::_comment ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* comment = state.GetValue < cc8* >( 1, 0 );
	if ( comment ) {
		MOAITestMgr::Get ().Comment ( comment );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITestMgr::_endTest ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool result = state.GetValue < bool >( 1, false );
	MOAITestMgr::Get ().EndTest ( result );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITestMgr::_getTestList ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAITestMgr::Get ().PushTestList ( state );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITestMgr::_setKeywords ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITestMgr::_setStagingFunc ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAITestMgr::Get ().mStagingFunc.SetStrongRef ( state, 1 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITestMgr::_setTestFunc ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAITestMgr::Get ().mTestFunc.SetStrongRef ( state, 1 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITestMgr::_staging ( lua_State* L ) {
	MOAILuaState state ( L );
	
	state.Push ( true );
	return 1;
}

//================================================================//
// MOAITestMgr
//================================================================//

//----------------------------------------------------------------//
void MOAITestMgr::BeginTest ( cc8* testName ) {

	this->EndTest ( false );
	this->mTestName = testName;
	
	if ( this->mResultsFile && this->mTestName.size ()) {
	
		cc8* op = this->mStaging ? "STAGING" : "TEST";
	
		this->PrintResult ( "----------------------------------------------------------------\n" );
		this->PrintResult ( "BEGIN %s: %s\n", op, this->mTestName.c_str ());
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::Comment ( cc8* comment ) {

	if ( this->mResultsFile && comment && comment [ 0 ]) {
		this->PrintResult ( "    - %s\n", comment );
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::EndTest ( bool result ) {

	if ( this->mResultsFile && this->mTestName.size ()) {
	
		cc8* op = this->mStaging ? "STAGING" : "TEST";
		cc8* msg = result ? "SUCCESS" : "FAIL";
		
		this->PrintResult ( "END %s: %s (%s)\n\n", op, this->mTestName.c_str (), msg );
		this->mTestName.clear ();
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::Finish () {

	if ( this->mResultsFile ) {
		fclose ( this->mResultsFile );
		this->mResultsFile = 0;
	}
}

//----------------------------------------------------------------//
MOAITestMgr::MOAITestMgr () :
	mResultsFile ( 0 ),
	mStaging ( false ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITestMgr::~MOAITestMgr () {

	this->Finish ();
}

//----------------------------------------------------------------//
void MOAITestMgr::PrintResult ( cc8* format, ... ) {

	if ( !format ) return;
	if ( !format [ 0 ]) return;
	if ( !this->mResultsFile ) return;

	va_list args;
	va_start ( args, format );
	vfprintf ( this->mResultsFile, format, args );
	vfprintf ( stdout, format, args );
	va_end ( args );
}

//----------------------------------------------------------------//
void MOAITestMgr::PushTestList ( MOAILuaState& state ) {

	lua_newtable ( state );
	
	Iterator itr = this->mFactory.Begin ();
	for ( int i = 1; itr != this->mFactory.End (); ++itr, ++i ) {
		
		lua_pushstring ( state, itr->first.c_str ());
		lua_rawseti ( state, -2, i );
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "beginTest",				_beginTest },
		{ "comment",				_comment },
		{ "endTest",				_endTest },
		{ "getTestList",			_getTestList },
		{ "setKeywords",			_setKeywords },
		{ "setStagingFunc",			_setStagingFunc },
		{ "setTestFunc",			_setTestFunc },
		{ "staging",				_staging },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITestMgr::RunScript ( cc8* filename ) {

	this->Finish ();

	if ( !USFileSys::CheckFileExists ( filename )) return;
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	int status = luaL_loadfile ( state, filename );
	
	if ( status ) {
		state.PrintErrors ( USLog::CONSOLE, status );
	}
	else {
	
		state.DebugCall ( 0, 0 );
		
		if ( this->mResultsFilename.size ()) {
			this->mResultsFile = fopen ( this->mResultsFilename, "w" );
		}
		
		if ( this->mResultsFile ) {
			
			if ( this->mStagingFunc && this->mStaging ) {
				state.Push ( this->mStagingFunc );
				state.DebugCall ( 0, 0 );
			}

			if ( this->mTestFunc && !this->mStaging ) {
				state.Push ( this->mTestFunc );
				state.DebugCall ( 0, 0 );
			}

			fclose ( this->mResultsFile );
			this->mResultsFile = NULL;
		}
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::RunTest ( cc8* testname ) {
	
	MOAITest* test = this->mFactory.Create ( testname );
	if ( test ) {
		
		this->mResultsFile = fopen ( this->mResultsFilename, "w" );
		
		if ( this->mStaging ) {
			test->Staging ( *this );
		}
		else {
			test->Test ( *this );
		}

		if ( this->mResultsFile ) {
			fclose ( this->mResultsFile );
			this->mResultsFile = NULL;
		}
		delete test;
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::SetResultsFile ( cc8* filename ) {

	this->mResultsFilename = USFileSys::GetAbsoluteFilePath ( filename );
}

//----------------------------------------------------------------//
void MOAITestMgr::SetStaging () {

	this->mStaging = true;
}

