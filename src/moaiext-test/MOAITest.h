// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEST_H
#define	MOAITEST_H

#include <moaicore/moaicore.h>

#define TEST_NAME(name) cc8* GetName () { return name; }

class MOAITestMgr;

//================================================================//
// MOAITest
//================================================================//
class MOAITest {
public:

	//----------------------------------------------------------------//
	virtual cc8*		GetName				() = 0;
						MOAITest			();
	virtual				~MOAITest			();
	virtual void		Staging				( MOAITestMgr& testMgr );
	virtual void		Test				( MOAITestMgr& testMgr ) = 0;
};

#endif
