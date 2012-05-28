// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-test.h>
#include <moaiext-test/MOAITestMgr.h>

//================================================================//
// AKU-test
//================================================================//

//----------------------------------------------------------------//
void AKUTestInit () {

	MOAITestMgr::Affirm ();
	
	REGISTER_LUA_CLASS ( MOAITestMgr )
}

//----------------------------------------------------------------//
void AKUTestRunScript ( const char* filename ) {
	MOAITestMgr::Get ().RunScript ( filename );
}

//----------------------------------------------------------------//
void AKUTestSetResultsFile ( const char* filename ) {
	MOAITestMgr::Get ().SetResultsFile ( filename );
}

//----------------------------------------------------------------//
void AKUTestSetStaging () {
	MOAITestMgr::Get ().SetStaging ();
}
