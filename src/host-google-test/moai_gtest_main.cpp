//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>

#include <moai-core/host.h>
#include <moai-sdl/host.h>
#include <host-modules/aku_modules.h>

//================================================================//
// GTESTMoaiContext
//================================================================//
class GTESTMoaiContext: public ::testing::Test {
public:

	AKUContextID	mContext;

	//----------------------------------------------------------------//
	GTESTMoaiContext () :
		mContext ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~GTESTMoaiContext () {
	}
 
	//----------------------------------------------------------------//
	void SetUp () {
	
		this->mContext = AKUCreateContext ();
	}
 
	//----------------------------------------------------------------//
	void TearDown () {
	
		AKUDeleteContext ( this->mContext );
	}
};

//----------------------------------------------------------------//
TEST_F ( GTESTMoaiContext, MoaiContextTest ) {
	ASSERT_TRUE ( this->mContext != 0 );
}

//----------------------------------------------------------------//
int main ( int argc, char **argv ) {
	::testing::InitGoogleTest ( &argc, argv );
	
	AKUAppInitialize ();
	
	int result = RUN_ALL_TESTS ();
	
	AKUAppFinalize ();
	return result;
}
