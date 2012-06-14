// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIGlobals.h>

//================================================================//
// MOAIGlobalClassFinalizer
//================================================================//

//----------------------------------------------------------------//
void MOAIGlobalClassFinalizer::OnGlobalsFinalize () {
}

//----------------------------------------------------------------//
void MOAIGlobalClassFinalizer::OnGlobalsRestore () {
}

//----------------------------------------------------------------//
void MOAIGlobalClassFinalizer::OnGlobalsRetire () {
}

//----------------------------------------------------------------//
MOAIGlobalClassFinalizer::MOAIGlobalClassFinalizer () {

	MOAIGlobals* globals = MOAIGlobalsMgr::Get ();
	assert ( globals );
	
	this->mNext = globals->mFinalizers;
	globals->mFinalizers = this;
}

//----------------------------------------------------------------//
MOAIGlobalClassFinalizer::~MOAIGlobalClassFinalizer () {

	MOAIGlobals* globals = MOAIGlobalsMgr::Get ();
	assert ( globals );
	
	MOAIGlobalClassFinalizer* cursor = globals->mFinalizers;
	globals->mFinalizers = 0;
	
	while ( cursor ) {
		MOAIGlobalClassFinalizer* finalizer = cursor;
		cursor = cursor->mNext;
		
		if ( finalizer != this ) {
			finalizer->mNext = globals->mFinalizers;
			globals->mFinalizers = finalizer;
		}
	}
}

//================================================================//
// MOAIGlobals
//================================================================//

//----------------------------------------------------------------//
void MOAIGlobals::Restore () {

	MOAIGlobalClassFinalizer* finalizer = this->mFinalizers;
	for ( ; finalizer; finalizer = finalizer->mNext ) {
		finalizer->OnGlobalsRestore ();
	}
}

//----------------------------------------------------------------//
void MOAIGlobals::Retire () {

	MOAIGlobalClassFinalizer* finalizer = this->mFinalizers;
	for ( ; finalizer; finalizer = finalizer->mNext ) {
		finalizer->OnGlobalsRetire ();
	}
}

//----------------------------------------------------------------//
MOAIGlobals::MOAIGlobals () :
	mFinalizers ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGlobals::~MOAIGlobals () {

	MOAIGlobalClassFinalizer* finalizer = this->mFinalizers;
	for ( ; finalizer; finalizer = finalizer->mNext ) {
		finalizer->OnGlobalsFinalize ();
	}
	this->mFinalizers = 0;
	
	u32 total = this->mGlobals.Size ();
	for ( u32 i = 1; i <= total; ++i ) {
		MOAIGlobalPair& pair = this->mGlobals [ total - i ];
		MOAIObject* object = pair.mObject;
		
		pair.mObject = 0;
		pair.mPtr = 0;
		
		if ( object ) {
			object->Release ();
		}
	}
}


//================================================================//
// MOAIGlobalsMgr
//================================================================//

MOAIGlobalsMgr::GlobalsSet* MOAIGlobalsMgr::sGlobalsSet = 0;
MOAIGlobals* MOAIGlobalsMgr::sInstance = 0;

//----------------------------------------------------------------//
MOAIGlobals* MOAIGlobalsMgr::Create () {
	
	if ( !sGlobalsSet ) {
		sGlobalsSet = new GlobalsSet ();
	}

	MOAIGlobals* globals = new MOAIGlobals ();
	sGlobalsSet->insert ( globals );
	sInstance = globals;

	return globals;
}

//----------------------------------------------------------------//
void MOAIGlobalsMgr::Delete ( MOAIGlobals* globals ) {
	
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
void MOAIGlobalsMgr::Finalize () {

	if ( sGlobalsSet ) {

		GlobalsSetIt globalsIt = sGlobalsSet->begin ();
		for ( ; globalsIt != sGlobalsSet->end (); ++globalsIt ) {
			MOAIGlobals* globals = *globalsIt;
			delete globals;
		}
		
		sGlobalsSet->clear ();
		sInstance = 0;
		
		delete sGlobalsSet;
		sGlobalsSet = 0;
	}
}

//----------------------------------------------------------------//
MOAIGlobals* MOAIGlobalsMgr::Get () {

	return sInstance;
}

//----------------------------------------------------------------//
void MOAIGlobalsMgr::Set ( MOAIGlobals* globals ) {

	sInstance = globals;
}

//----------------------------------------------------------------//
MOAIGlobalsMgr::MOAIGlobalsMgr () {
}

//----------------------------------------------------------------//
MOAIGlobalsMgr::~MOAIGlobalsMgr () {
}
