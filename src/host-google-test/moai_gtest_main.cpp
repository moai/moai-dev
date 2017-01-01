//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#include "moai_gtest.h"

//----------------------------------------------------------------//
int main ( int argc, char **argv ) {
	::testing::InitGoogleTest ( &argc, argv );
	
	AKUAppInitialize ();
	
	int result = RUN_ALL_TESTS ();
	
	AKUAppFinalize ();
	return result;
}
