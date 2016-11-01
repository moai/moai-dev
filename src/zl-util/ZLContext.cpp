// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLContext.h>
#include <zl-util/ZLLog.h>

//================================================================//
// ZLContextClassBase
//================================================================//

//----------------------------------------------------------------//
void ZLContextClassBase::OnGlobalsFinalize () {
}

//----------------------------------------------------------------//
void ZLContextClassBase::OnGlobalsInitialize () {
}

//----------------------------------------------------------------//
ZLContextClassBase::ZLContextClassBase () {
}

//----------------------------------------------------------------//
ZLContextClassBase::~ZLContextClassBase () {
}

//================================================================//
// ZLContexts
//================================================================//

//----------------------------------------------------------------//
ZLContexts::ZLContexts () {
}

//----------------------------------------------------------------//
ZLContexts::~ZLContexts () {

	ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContexts::~ZLContexts %p\n", this );

	size_t total = this->mGlobals.Size ();
	
	// TODO: move to higher level context cleanup; ZLContexts should not know about this
	//MOAILuaRuntime::Get ().Close (); // call this ahead of everything to purge all the Lua bindings
	
	// Lua runtime gets special treatment
	//u32 luaRuntimeID = ZLContextID < MOAILuaRuntime >::GetID ();
	//this->mGlobals [ luaRuntimeID ].mIsValid = false;
	
	// finalize everything
	for ( size_t i = 1; i <= total; ++i ) {
		ZLContextPair& pair = this->mGlobals [ total - i ];
		ZLContextClassBase* global = pair.mGlobalBase;

		if ( global ) {
			ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContexts: finalizing global %p\n", global );
			global->OnGlobalsFinalize ();
		}
	}
	
	// mark everything as invalid
	for ( size_t i = 1; i <= total; ++i ) {
	
		ZLContextPair& pair = this->mGlobals [ total - i ];
		ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContexts: invalidating global %p\n", pair.mGlobalBase );
	
		pair.mIsValid = false;
	}

	// and officially delete everything
	for ( size_t i = 1; i <= total; ++i ) {
		ZLContextPair& pair = this->mGlobals [ total - i ];
		ZLContextClassBase* global = pair.mGlobalBase;

		if ( global ) {
			ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContexts: deleting global %p\n", global );
			delete global;
		}
	}
}

//================================================================//
// ZLContextsMgr
//================================================================//

ZLThreadLocalPtr < ZLContextsMgr::GlobalsSet >		ZLContextsMgr::sGlobalsSet;
ZLThreadLocalPtr < ZLContexts >						ZLContextsMgr::sInstance;

//----------------------------------------------------------------//
bool ZLContextsMgr::Check ( ZLContexts* globals ) {

	GlobalsSet* globalSet = sGlobalsSet.Get ();
	return globalSet ? globalSet->contains ( globals ) : false;
}

//----------------------------------------------------------------//
ZLContexts* ZLContextsMgr::Create () {
	
	GlobalsSet* globalSet = sGlobalsSet.Get ();
	
	if ( !globalSet ) {
		globalSet = new GlobalsSet ();
		sGlobalsSet.Set ( globalSet );
	}

	ZLContexts* globals = new ZLContexts ();
	
	ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContextsMgr: new context %p\n", globals );
	
	globalSet->insert ( globals );
	sInstance.Set ( globals );

	return globals;
}

//----------------------------------------------------------------//
void ZLContextsMgr::Delete ( ZLContexts* globals ) {
	
	ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContextsMgr: deleting context %p\n", globals );
	
	GlobalsSet* globalSet = sGlobalsSet.Get ();
	ZLContexts* prevInstance = sInstance.Get ();
	
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
void ZLContextsMgr::Finalize () {

	ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContextsMgr: finalizing\n" );

	GlobalsSet* globalSet = sGlobalsSet.Get ();

    if ( globalSet ) {

        GlobalsSetIt globalsIt = globalSet->begin ();
        for ( ; globalsIt != globalSet->end (); ++globalsIt ) {
		
			ZLContexts* instance = *globalsIt;
			
            sInstance.Set ( instance );
			
			ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContextsMgr: deleting context %p\n", instance );
			
            delete instance;
        }
        
        globalSet->clear ();
        sInstance.Set ( 0 );
        
        delete globalSet;
        sGlobalsSet.Set ( 0 );
    }
}

//----------------------------------------------------------------//
ZLContexts* ZLContextsMgr::Get () {

	return sInstance.Get ();
}

//----------------------------------------------------------------//
ZLContexts* ZLContextsMgr::Set ( ZLContexts* globals ) {

//	ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContextsMgr: setting context %p\n", globals );

	ZLContexts* prev = sInstance.Get ();
	sInstance.Set ( globals );
	return prev;
}

//----------------------------------------------------------------//
ZLContextsMgr::ZLContextsMgr () {
}

//----------------------------------------------------------------//
ZLContextsMgr::~ZLContextsMgr () {
}

//================================================================//
// ZLScopedContext
//================================================================//

//----------------------------------------------------------------//
void ZLScopedContext::Clear () {

	this->mOriginalContext = 0;
}

//----------------------------------------------------------------//
ZLScopedContext::ZLScopedContext () {

	this->mOriginalContext = ZLContextsMgr::Get ();
}

//----------------------------------------------------------------//
ZLScopedContext::~ZLScopedContext () {

	ZLContextsMgr::Set ( this->mOriginalContext );
}
