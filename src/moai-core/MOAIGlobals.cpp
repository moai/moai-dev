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

ZLThreadLocalPtr < MOAIGlobalsMgr::GlobalsSet >		MOAIGlobalsMgr::sGlobalsSet;
ZLThreadLocalPtr < MOAIGlobals >					MOAIGlobalsMgr::sInstance;

//----------------------------------------------------------------//
bool MOAIGlobalsMgr::Check ( MOAIGlobals* globals ) {

	GlobalsSet* globalSet = sGlobalsSet.Get ();
	return globalSet ? globalSet->contains ( globals ) : false;
}

//----------------------------------------------------------------//
MOAIGlobals* MOAIGlobalsMgr::Create () {
	
	GlobalsSet* globalSet = sGlobalsSet.Get ();
	
	if ( !globalSet ) {
		globalSet = new GlobalsSet ();
		sGlobalsSet.Set ( globalSet );
	}

	MOAIGlobals* globals = new MOAIGlobals ();
	
	ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: new context %p\n", globals );
	
	globalSet->insert ( globals );
	sInstance.Set ( globals );

	return globals;
}

//----------------------------------------------------------------//
void MOAIGlobalsMgr::Delete ( MOAIGlobals* globals ) {
	
	ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: deleting context %p\n", globals );
	
	GlobalsSet* globalSet = sGlobalsSet.Get ();
	MOAIGlobals* prevInstance = sInstance.Get ();
	
	if ( globalSet ) {
		if ( globalSet->contains ( globals )) {
			
			sInstance.Set ( globals );
			delete globals;
			globalSet->erase ( globals );
			sInstance.Set ( 0 );
		}
	}
	
	// don't set this to nil until *after* deleting it!
	if ( prevInstance != globals ) {
		sInstance.Set ( prevInstance );
	}
}

//----------------------------------------------------------------//
void MOAIGlobalsMgr::Finalize () {

	ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: finalizing\n" );

	GlobalsSet* globalSet = sGlobalsSet.Get ();

    if ( globalSet ) {

        GlobalsSetIt globalsIt = globalSet->begin ();
        for ( ; globalsIt != globalSet->end (); ++globalsIt ) {
		
			MOAIGlobals* instance = *globalsIt;
			
            sInstance.Set ( instance );
			
			ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: deleting context %p\n", instance );
			
            delete instance;
        }
        
        globalSet->clear ();
        sInstance.Set ( 0 );
        
        delete globalSet;
        sGlobalsSet.Set ( 0 );
    }
}

//----------------------------------------------------------------//
MOAIGlobals* MOAIGlobalsMgr::Get () {

	return sInstance.Get ();
}

//----------------------------------------------------------------//
MOAIGlobals* MOAIGlobalsMgr::Set ( MOAIGlobals* globals ) {

//	ZLLog_DebugF ( ZLLog::CONSOLE, "MOAIGlobalsMgr: setting context %p\n", globals );

	MOAIGlobals* prev = sInstance.Get ();
	sInstance.Set ( globals );
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
