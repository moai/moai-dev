// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/STLSet.h>
#include <uslscore/USGlobals.h>

typedef STLSet < USGlobals* >::iterator GlobalsSetIt;
typedef STLSet < USGlobals* > GlobalsSet;

static GlobalsSet* sGlobalsSet = 0;
static USGlobals* sInstance = 0;

//================================================================//
// USGlobals
//================================================================//

//----------------------------------------------------------------//
USGlobals* USGlobals::Create () {

	if ( !sGlobalsSet ) {
		sGlobalsSet = new GlobalsSet ();
	}

	USGlobals* globals = new USGlobals ();
	sGlobalsSet->insert ( globals );
	sInstance = globals;

	return globals;
}

//----------------------------------------------------------------//
void USGlobals::Delete ( USGlobals* globals ) {
	
	if ( sGlobalsSet ) {
		if ( sGlobalsSet->contains ( globals )) {
			sGlobalsSet->erase ( globals );
			delete globals;
		}
	}
	
	// don't get this to nil until *after* deleting it!
	if ( globals == sInstance ) {
		sInstance = 0;
	}
}

//----------------------------------------------------------------//
void USGlobals::Finalize () {

	if ( sGlobalsSet ) {

		GlobalsSetIt globalsIt = sGlobalsSet->begin ();
		for ( ; globalsIt != sGlobalsSet->end (); ++globalsIt ) {
			USGlobals* globals = *globalsIt;
			delete globals;
		}
		
		sGlobalsSet->clear ();
		sInstance = 0;
		
		delete sGlobalsSet;
		sGlobalsSet = 0;
	}
}

//----------------------------------------------------------------//
USGlobals* USGlobals::Get () {

	return sInstance;
}

//----------------------------------------------------------------//
void USGlobals::Set ( USGlobals* globals ) {

	sInstance = globals;
}

//----------------------------------------------------------------//
USGlobals::USGlobals () {
}

//----------------------------------------------------------------//
USGlobals::~USGlobals () {

	u32 total = this->mGlobals.Size ();
	for ( u32 i = 1; i <= total; ++i ) {
		USGlobalPair& pair = this->mGlobals [ total - i ];
		
		USObject* object = pair.mObject;
		pair.mObject = 0;
		pair.mPtr = 0;
		
		if ( object ) {
			object->Release ();
		}
	}
}
