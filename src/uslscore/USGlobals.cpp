// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/STLSet.h>
#include <uslscore/USGlobals.h>

//================================================================//
// USGlobalsFinalizer
//================================================================//

//----------------------------------------------------------------//
USGlobalsFinalizer::USGlobalsFinalizer () {
}

//----------------------------------------------------------------//
USGlobalsFinalizer::~USGlobalsFinalizer () {
}

//================================================================//
// USGlobals
//================================================================//

//----------------------------------------------------------------//
USGlobals::USGlobals () {
}

//----------------------------------------------------------------//
USGlobals::~USGlobals () {

	FinalizersIt finalizersIt = this->mFinalizers.begin ();
	for ( ; finalizersIt != this->mFinalizers.end (); ++finalizersIt ) {
		USGlobalsFinalizer* finalizer = ( *finalizersIt );
		finalizer->OnFinalize ();
		delete finalizer;
	}
	this->mFinalizers.clear ();

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


//================================================================//
// USGlobalsMgr
//================================================================//

USGlobalsMgr::GlobalsSet* USGlobalsMgr::sGlobalsSet = 0;
USGlobals* USGlobalsMgr::sInstance = 0;

//----------------------------------------------------------------//
USGlobals* USGlobalsMgr::Create () {
	
	if ( !sGlobalsSet ) {
		sGlobalsSet = new GlobalsSet ();
	}

	USGlobals* globals = new USGlobals ();
	sGlobalsSet->insert ( globals );
	sInstance = globals;

	return globals;
}

//----------------------------------------------------------------//
void USGlobalsMgr::Delete ( USGlobals* globals ) {
	
	if ( sGlobalsSet ) {
		if ( sGlobalsSet->contains ( globals )) {
			sGlobalsSet->erase ( globals );
			delete globals;
		}
	}
	
	// don't set this to nil until *after* deleting it!
	if ( globals == sInstance ) {
		sInstance = 0;
	}
}

//----------------------------------------------------------------//
void USGlobalsMgr::Finalize () {

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
USGlobals* USGlobalsMgr::Get () {

	return sInstance;
}

//----------------------------------------------------------------//
void USGlobalsMgr::Set ( USGlobals* globals ) {

	sInstance = globals;
}

//----------------------------------------------------------------//
USGlobalsMgr::USGlobalsMgr () {
}

//----------------------------------------------------------------//
USGlobalsMgr::~USGlobalsMgr () {
}
