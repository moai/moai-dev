// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEST_SAMPLE_H
#define	MOAITEST_SAMPLE_H

#include <moai-core/headers.h>
#include <moai-test/MOAITest.h>
#include <moai-test/MOAITestKeywords.h>
#include <moai-test/MOAITestMgr.h>

//================================================================//
// MOAITest_sample
//================================================================//
class MOAITest_sample :
	public MOAITest {
public:

	TEST_NAME ( "sample" )

	//----------------------------------------------------------------//
	void Staging ( MOAITestMgr& testMgr ) {
		
		testMgr.SetFilter ( MOAI_TEST_UTIL, MOAI_TEST_SAMPLE, 0 );
		
		testMgr.BeginTest ( "sample" );
		testMgr.Success ( "comment" );
		testMgr.EndTest ( true );
	}
	
	//----------------------------------------------------------------//
	void Test ( MOAITestMgr& testMgr ) {
		
		testMgr.BeginTest ( "sample" );
		testMgr.Success ( "comment" );
		testMgr.EndTest ( true );
	}
};

#endif
