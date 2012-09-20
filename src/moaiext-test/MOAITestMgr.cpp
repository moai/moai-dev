// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-test/MOAITestKeywords.h>
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
int MOAITestMgr::_checkFilter ( lua_State* L ) {
	MOAILuaState state ( L );

	state.Push ( MOAITestMgr::Get ().CheckFilter ());
	return 1;
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
int MOAITestMgr::_failure ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* type = state.GetValue < cc8* >( 1, "" );
	cc8* detail = state.GetValue < cc8* >( 2, "" );

	MOAITestMgr::Get ().Failure ( type, detail );
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
int MOAITestMgr::_setFilter ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAITestMgr::Get ().SetFilter ( state.GetValue < cc8* >( 1, "" ));
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

	if ( this->mXmlWriter ) {
		mXmlWriter->AddElement ( "testsuite" );
	}
}

//----------------------------------------------------------------//
bool MOAITestMgr::CheckFilter () {

	if ( this->mFilter.size () == 0 ) return true; // no filter

	USFileStream fileStream;
	if ( fileStream.OpenRead ( this->mFilterFilename )) {
		
		STLSet < STLString > keywords;
		MOAITestMgr::ParseKeywords ( fileStream, keywords );
		
		STLSet < STLString >::iterator itr;
		for ( itr = this->mFilter.begin (); itr != this->mFilter.end (); ++itr ) {
			if ( keywords.contains ( *itr )) return true;
		}
	}
	return false;
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

	if ( this->mXmlWriter ) {
		mXmlWriter->CloseElement ();
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::ExtendFilter ( cc8* filter ) {
	
	if ( !filter ) return;
	if ( !filter [ 0 ]) return;
	
	USByteStream filterStream;
	size_t filterLength = strlen ( filter );
	filterStream.SetBuffer (( void* )filter, filterLength, filterLength );
	MOAITestMgr::ParseKeywords ( filterStream, this->mFilter );
}

//----------------------------------------------------------------//
void MOAITestMgr::Failure ( cc8* type, cc8* detail ) {
	
	if ( this->mResultsFile && this->mTestName.size ()) {

		this->PrintResult ( "    - FAILED\t%s\t%s\n", type, detail );
	}

	if ( this->mXmlWriter && this->mTestName.size ()) {

		mXmlWriter->AddElement ( "testcase" );
		mXmlWriter->AddAttribute ( "classname", this->mTestName );
		mXmlWriter->AddAttribute ( "name", detail );

		mXmlWriter->AddElement ( "failure" );
		mXmlWriter->AddAttribute ( "type", type );
		mXmlWriter->AddText ( detail );
		mXmlWriter->CloseElement ();

		mXmlWriter->CloseElement ();
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
void MOAITestMgr::ParseKeywords ( USStream& stream, STLSet < STLString >& keywords ) {

	bool more = true;
	while ( more ) {
		
		STLString keyword = stream.ReadToken ( " ,;\t\r\n" );
		
		more = keyword.length () > 0;
		if ( more ) {
			keywords.insert ( keyword );
		}
	}
}

//----------------------------------------------------------------//
MOAITestMgr::MOAITestMgr () :
	mResultsFile ( 0 ),
	mXmlWriter ( 0 ),
	mStaging ( false ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITestMgr::~MOAITestMgr () {

	this->Finish ();

	if ( this->mXmlWriter ) {

		mXmlWriter->SaveDocument ( this->mXmlResultsFilename );
		delete this->mXmlWriter;
	}
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
	
	state.SetField ( -1, "MATH",		MOAI_TEST_MATH );
	state.SetField ( -1, "SAMPLE",		MOAI_TEST_SAMPLE );
	state.SetField ( -1, "UTIL",		MOAI_TEST_UTIL );
	
	luaL_Reg regTable [] = {
		{ "beginTest",				_beginTest },
		{ "checkFilter",			_checkFilter },
		{ "comment",				_comment },
		{ "endTest",				_endTest },
		{ "failure",				_failure },
		{ "getTestList",			_getTestList },
		{ "setFilter",				_setFilter },
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
		
		this->mResultsFile = fopen ( this->mResultsFilename, "w" );
		
		if ( this->mStaging ) {
			
			if ( this->mStagingFunc ) {
				state.Push ( this->mStagingFunc );
				state.DebugCall ( 0, 0 );
			}
			this->WriteFilterFile ();
		}
		else {
			
			if ( this->mTestFunc ) {
				state.Push ( this->mTestFunc );
				state.DebugCall ( 0, 0 );
			}
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
			this->WriteFilterFile ();
		}
		else {
			test->Test ( *this );
		}

		delete test;
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::SetFilter ( cc8* filter, cc8* next, ... ) {

	this->mFilter.clear ();
	
	this->ExtendFilter ( filter );
	
	if ( next ) {
	
		this->ExtendFilter ( next );
		
		va_list args;
		va_start ( args, next );
		
		bool more = true;
		while ( next ) {
			next = va_arg ( args, cc8* );
			this->ExtendFilter ( next );
		}
		
		va_end ( args );
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::SetFilterFile ( cc8* filename ) {

	this->mFilterFilename = filename;
}

//----------------------------------------------------------------//
void MOAITestMgr::SetResultsFile ( cc8* filename ) {

	this->mResultsFilename = USFileSys::GetAbsoluteFilePath ( filename );
}

//----------------------------------------------------------------//
void MOAITestMgr::SetXmlResultsFile ( cc8* filename ) {

	this->mXmlResultsFilename = USFileSys::GetAbsoluteFilePath ( filename );
	this->mXmlWriter = new MOAIXmlWriter ();
}

//----------------------------------------------------------------//
void MOAITestMgr::SetStaging () {

	this->mStaging = true;
}

//----------------------------------------------------------------//
void MOAITestMgr::Success ( cc8* detail ) {

	if ( this->mResultsFile && detail && detail [ 0 ]) {

		this->PrintResult ( "    - %s\n", detail );
	}

	if ( this->mXmlWriter && this->mTestName.size ()) {

		mXmlWriter->AddElement ( "testcase" );
		mXmlWriter->AddAttribute ( "classname", this->mTestName );
		mXmlWriter->AddAttribute ( "name", detail );
		mXmlWriter->CloseElement ();
	}
}

//----------------------------------------------------------------//
void MOAITestMgr::WriteFilterFile () {

	if ( this->mFilter.size ()) {
		USFileStream stream;
		if ( stream.OpenWrite ( this->mFilterFilename )) {
			STLSet < STLString >::iterator itr;
			for ( itr = this->mFilter.begin (); itr != this->mFilter.end (); ++itr ) {
				stream.Print ( "%s\n", ( *itr ).str ());
			}
		}
	}
}