//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAI_GTEST_H
#define	MOAI_GTEST_H

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
class GTESTMoaiContext:
	public ::testing::Test {
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

#endif
