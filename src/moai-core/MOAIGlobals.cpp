// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-core/MOAILuaRuntime.h>
#include <moai-core/MOAIGlobals.h>

//================================================================//
// MOAIGlobalClassBase
//================================================================//

//----------------------------------------------------------------//
void MOAIGlobalClassBase::OnGlobalsFinalize () {
}

//----------------------------------------------------------------//
void MOAIGlobalClassBase::OnGlobalsInitialize () {
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

	ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobals::~MOAIGlobals %p\n", this );

	size_t total = this->mGlobals.Size ();
	
	MOAILuaRuntime::Get ().Close (); // call this ahead of everything to purge all the Lua bindings
	
	// Lua runtime gets special treatment
	u32 luaRuntimeID = MOAIGlobalID < MOAILuaRuntime >::GetID ();
	this->mGlobals [ luaRuntimeID ].mIsValid = false;
	
	// finalize everything
	for ( size_t i = 1; i <= total; ++i ) {
		MOAIGlobalPair& pair = this->mGlobals [ total - i ];
		MOAIGlobalClassBase* global = pair.mGlobalBase;

		if ( global ) {
			ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobals: finalizing global %p\n", global );
			global->OnGlobalsFinalize ();
		}
	}
	
	// mark everything as invalid
	for ( size_t i = 1; i <= total; ++i ) {
	
		MOAIGlobalPair& pair = this->mGlobals [ total - i ];
		ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobals: invalidating global %p\n", pair.mGlobalBase );
	
		pair.mIsValid = false;
	}

	// and officially delete everything
	for ( size_t i = 1; i <= total; ++i ) {
		MOAIGlobalPair& pair = this->mGlobals [ total - i ];
		MOAIGlobalClassBase* global = pair.mGlobalBase;

		if ( global ) {
			ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobals: deleting global %p\n", global );
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
	
	ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: new context %p\n", globals );
	
	sGlobalsSet->insert ( globals );
	sInstance = globals;

	return globals;
}

//----------------------------------------------------------------//
void MOAIGlobalsMgr::Delete ( MOAIGlobals* globals ) {
	
	ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: deleting context %p\n", globals );
	
	MOAIGlobals* prevInstance = sInstance;
	
	if ( sGlobalsSet ) {
		if ( sGlobalsSet->contains ( globals )) {
			
			sInstance = globals;
			delete globals;
			sGlobalsSet->erase ( globals );
			sInstance = 0;
		}
	}
	
	// don't set this to nil until *after* deleting it!
	if ( prevInstance == globals ) {
		sInstance = prevInstance;
	}
}

//----------------------------------------------------------------//
void MOAIGlobalsMgr::Finalize () {

	ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: finalizing\n" );

    if ( sGlobalsSet ) {

        GlobalsSetIt globalsIt = sGlobalsSet->begin ();
        for ( ; globalsIt != sGlobalsSet->end (); ++globalsIt ) {
            sInstance = *globalsIt;
			
			ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: deleting context %p\n", sInstance );
			
            delete sInstance;
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

//	ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: setting context %p\n", globals );

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
void MOAIScopedContext::Clear () {

	this->mOriginalContext = 0;
}

//----------------------------------------------------------------//
MOAIScopedContext::MOAIScopedContext () {

	this->mOriginalContext = MOAIGlobalsMgr::Get ();
}

//----------------------------------------------------------------//
MOAIScopedContext::~MOAIScopedContext () {

	MOAIGlobalsMgr::Set ( this->mOriginalContext );
}
