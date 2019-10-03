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

#include <moai-core/headers.h>
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

//================================================================//
// GTESTLifecycleCounter
//================================================================//
class GTESTLifecycleCounter {
private:

	friend class GTESTLifecycleTracker;

	int mConstructed;
	int mDestructed;

public:

	//----------------------------------------------------------------//
	int Alive () {
		return this->mConstructed - this->mDestructed;
	}
	
	//----------------------------------------------------------------//
	void Construct () {
		this->mConstructed++;
	}
	
	//----------------------------------------------------------------//
	void Destruct () {
		this->mDestructed++;
	}
	
	//----------------------------------------------------------------//
	GTESTLifecycleCounter () :
		mConstructed ( 0 ),
		mDestructed ( 0 ) {
	}
};

//================================================================//
// GTESTLifecycleTracker
//================================================================//
class GTESTLifecycleTracker {
private:

	STLMap < std::string, GTESTLifecycleCounter >	mLifecycle;

public:

	//----------------------------------------------------------------//
	int Alive ( std::string classname ) {
		int alive = this->mLifecycle [ classname ].Alive ();
		printf ( "ALIVE %s: %d!\n", classname.c_str (), alive );
		return alive;
	}

	//----------------------------------------------------------------//
	bool Check ( std::string classname, int constructed, int destructed, int alive ) {
		GTESTLifecycleCounter& count = this->mLifecycle [ classname ];
		return (
			( count.mConstructed == constructed ) &&
			( count.mDestructed == destructed ) &&
			( count.Alive () == alive )
		);
	}

	//----------------------------------------------------------------//
	void Construct ( std::string classname ) {
		printf ( "CONSTRUCTED %s!\n", classname.c_str ());
		this->mLifecycle [ classname ].Construct ();
	}

	//----------------------------------------------------------------//
	int Constructed ( std::string classname ) {
		return this->mLifecycle [ classname ].mConstructed;
	}

	//----------------------------------------------------------------//
	void Destruct ( std::string classname ) {
		printf ( "DESTRUCTED %s!\n", classname.c_str ());
		this->mLifecycle [ classname ].Destruct ();
	}
	
	//----------------------------------------------------------------//
	int Destructed ( std::string classname ) {
		return this->mLifecycle [ classname ].mDestructed;
	}
};

#endif
