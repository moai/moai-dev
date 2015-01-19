// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-core/MOAIGlobals.h>

//================================================================//
// MOAIGlobalClassBase
//================================================================//

//----------------------------------------------------------------//
void MOAIGlobalClassBase::OnGlobalsFinalize () {
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
MOAIGlobals::MOAIGlobals () {
}

//----------------------------------------------------------------//
MOAIGlobals::~MOAIGlobals () {

	size_t total = this->mGlobals.Size ();
	
	// finalize everything
	for ( size_t i = 1; i <= total; ++i ) {
		MOAIGlobalPair& pair = this->mGlobals [ total - i ];
		MOAIGlobalClassBase* global = pair.mGlobalBase;

		if ( global ) {
			global->OnGlobalsFinalize ();
		}
	}
	
	// marke everything as invalid
	for ( size_t i = 1; i <= total; ++i ) {
		this->mGlobals [ total - i ].mIsValid = false;
	}

	// and officially delete everything
	for ( size_t i = 1; i <= total; ++i ) {
		MOAIGlobalPair& pair = this->mGlobals [ total - i ];
		MOAIGlobalClassBase* global = pair.mGlobalBase;

		if ( global ) {
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
bool MOAIGlobalsMgr::Check ( MOAIGlobals* globals ) {

	return sGlobalsSet->contains ( globals );
}

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
MOAIGlobals* MOAIGlobalsMgr::Set ( MOAIGlobals* globals ) {

	MOAIGlobals* prev = sInstance;
	sInstance = globals;
	return prev;
}

//----------------------------------------------------------------//
MOAIGlobalsMgr::MOAIGlobalsMgr () {
}

//----------------------------------------------------------------//
MOAIGlobalsMgr::~MOAIGlobalsMgr () {
}

//================================================================//
// MOAIScopedContext
//================================================================//
	
//----------------------------------------------------------------//
MOAIScopedContext::MOAIScopedContext () {

	this->mOriginalContext = MOAIGlobalsMgr::Get ();
}

//----------------------------------------------------------------//
MOAIScopedContext::~MOAIScopedContext () {

	MOAIGlobalsMgr::Set ( this->mOriginalContext );
}
