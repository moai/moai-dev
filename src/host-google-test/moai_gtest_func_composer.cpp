//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"

//----------------------------------------------------------------//
template < typename FUNC >
class Composer {
public:

	//----------------------------------------------------------------//
	template < typename TYPE >
	void Op () {
		
	}
};


//----------------------------------------------------------------//
TEST_F ( GTESTMoaiContext, func_composer ) {

	FinalWithFooBarA fa;
	fa.SayHello ();

	FinalWithFooBarB fb;
	fb.SayHello ();
	
	FinalWithConcreteSubclass fsc;
	fsc.SayHello ();
}
