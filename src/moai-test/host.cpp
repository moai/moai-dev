// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-test/host.h>
#include <moai-test/MOAITestMgr.h>

#include <moai-test/MOAITest_sample.h>
#include <moai-test/MOAITest_ZLQuaternion.h>

//================================================================//
// aku-test
//================================================================//

//----------------------------------------------------------------//
void AKUTestAppFinalize () {
}

//----------------------------------------------------------------//
void AKUTestAppInitialize () {
}

//----------------------------------------------------------------//
void AKUTestContextInitialize () {

	MOAITestMgr::Affirm ();
	
	REGISTER_LUA_CLASS ( MOAITestMgr )
	REGISTER_MOAI_TEST ( MOAITest_sample )
	REGISTER_MOAI_TEST ( MOAITest_USQuaternion )
}

//----------------------------------------------------------------//
void AKUTestRunScript ( const char* filename ) {
	MOAITestMgr::Get ().RunScript ( filename );
}

//----------------------------------------------------------------//
void AKUTestRunTest ( const char* testname ) {
	MOAITestMgr::Get ().RunTest ( testname );
}

//----------------------------------------------------------------//
void AKUTestSetFilter ( const char* filter ) {
	MOAITestMgr::Get ().SetFilter ( filter );
}

//----------------------------------------------------------------//
void AKUTestSetFilterFile ( const char* filename ) {
	MOAITestMgr::Get ().SetFilterFile ( filename );
}

//----------------------------------------------------------------//
void AKUTestSetResultsFile ( const char* filename ) {
	MOAITestMgr::Get ().SetResultsFile ( filename );
}

//----------------------------------------------------------------//
void AKUTestSetXmlResultsFile ( const char* filename ) {
	MOAITestMgr::Get ().SetXmlResultsFile ( filename );
}

//----------------------------------------------------------------//
void AKUTestSetStaging () {
	MOAITestMgr::Get ().SetStaging ();
}
