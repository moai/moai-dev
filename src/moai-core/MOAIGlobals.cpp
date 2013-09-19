// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-core/MOAIGlobals.h>

//================================================================//
// MOAIGlobalClassFinalizer
//================================================================//

//----------------------------------------------------------------//
void MOAIGlobalClassBase::OnGlobalsFinalize () {
}

//----------------------------------------------------------------//
void MOAIGlobalClassBase::OnGlobalsRestore () {
}

//----------------------------------------------------------------//
void MOAIGlobalClassBase::OnGlobalsRetire () {
}

//----------------------------------------------------------------//
MOAIGlobalClassBase::MOAIGlobalClassBase () {
}

//----------------------------------------------------------------//
MOAIGlobalClassBase::~MOAIGlobalClassBase () {
}

//================================================================//
// MOAIGlobals
//================================================================//

//----------------------------------------------------------------//
void MOAIGlobals::Restore () {

	u32 total = this->mGlobals.Size ();
	for ( u32 i = 1; i <= total; ++i ) {
		MOAIGlobalPair& pair = this->mGlobals [ i ];
		MOAIGlobalClassBase* global = pair.mGlobal;
		if ( global ) {
			global->OnGlobalsRestore ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIGlobals::Retire () {

	u32 total = this->mGlobals.Size ();
	for ( u32 i = 1; i <= total; ++i ) {
		MOAIGlobalPair& pair = this->mGlobals [ total - i ];
		MOAIGlobalClassBase* global = pair.mGlobal;
		if ( global ) {
			global->OnGlobalsRetire ();
		}
	}
}

//----------------------------------------------------------------//
MOAIGlobals::MOAIGlobals () {
}

//----------------------------------------------------------------//
MOAIGlobals::~MOAIGlobals () {

	u32 total = this->mGlobals.Size ();
	for ( u32 i = 1; i <= total; ++i ) {
		MOAIGlobalPair& pair = this->mGlobals [ total - i ];
		MOAIGlobalClassBase* global = pair.mGlobal;

		pair.mIsValid = false;

		if ( global ) {
			global->OnGlobalsFinalize ();
			delete global;
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
