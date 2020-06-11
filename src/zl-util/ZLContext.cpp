// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLContext.h>
#include <zl-util/ZLLog.h>

//#define ZL_ENABLE_CONTEXT_DEBUG_LOG

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
// ZLContext
//================================================================//

//----------------------------------------------------------------//
ZLContext::ZLContext () {
}

//----------------------------------------------------------------//
ZLContext::~ZLContext () {

	// get the log object here, since it will be markes as invalid with all the others below
	// no longer safe to use the ZLLog macros
	#ifdef ZL_ENABLE_CONTEXT_DEBUG_LOG
		ZLLog& log = ZLLog::Get ();
		log.LogF ( ZLLog::LOG_DEBUG, ZLLog::CONSOLE, "ZLContext::~ZLContext %p\n", this );
	#endif

	size_t total = this->mGlobals.Size ();
	
	// TODO: move to higher level context cleanup; ZLContext should not know about this
	//MOAILuaRuntime::Get ().Close (); // call this ahead of everything to purge all the Lua bindings
	
	// Lua runtime gets special treatment
	//u32 luaRuntimeID = ZLContextClassID < MOAILuaRuntime >::GetID ();
	//this->mGlobals [ luaRuntimeID ].mIsValid = false;
	
	// finalize everything
	for ( size_t i = 1; i <= total; ++i ) {
		ZLContextPair& pair = this->mGlobals [ total - i ];
		ZLContextClassBase* global = pair.mGlobalBase;

		if ( global ) {
		
			#ifdef ZL_ENABLE_CONTEXT_DEBUG_LOG
				log.LogF ( ZLLog::LOG_DEBUG, ZLLog::CONSOLE, "ZLContext: finalizing global %p\n", global );
			#endif
			
			global->OnGlobalsFinalize ();
		}
	}
	
	// mark everything as invalid
	for ( size_t i = 1; i <= total; ++i ) {
	
		ZLContextPair& pair = this->mGlobals [ total - i ];
		
		#ifdef ZL_ENABLE_CONTEXT_DEBUG_LOG
			log.LogF ( ZLLog::LOG_DEBUG, ZLLog::CONSOLE, "ZLContext: invalidating global %p\n", pair.mGlobalBase );
		#endif
	
		pair.mIsValid = false;
	}

	// and officially delete everything
	for ( size_t i = 1; i <= total; ++i ) {
		ZLContextPair& pair = this->mGlobals [ total - i ];
		ZLContextClassBase* global = pair.mGlobalBase;

		if ( global ) {
		
			#ifdef ZL_ENABLE_CONTEXT_DEBUG_LOG
				log.LogF ( ZLLog::LOG_DEBUG, ZLLog::CONSOLE, "ZLContext: deleting global %p\n", global );
			#endif
			
			delete global;
		}
	}
}

//================================================================//
// ZLContextMgr
//================================================================//

#ifdef ZL_USE_CONTEXT_THREADLOCAL
	ZLThreadLocalPtr < ZLContextMgr::GlobalsSet >		ZLContextMgr::sGlobalsSet;
	ZLThreadLocalPtr < ZLContext >						ZLContextMgr::sInstance;
#else
	ZLLocalPtr < ZLContextMgr::GlobalsSet >				ZLContextMgr::sGlobalsSet;
	ZLLocalPtr < ZLContext >							ZLContextMgr::sInstance;
#endif

//----------------------------------------------------------------//
bool ZLContextMgr::Check ( ZLContext* globals ) {

	GlobalsSet* globalSet = sGlobalsSet.Get ();
	return globalSet ? globalSet->contains ( globals ) : false;
}

//----------------------------------------------------------------//
u32 ZLContextMgr::CountContexts () {

	GlobalsSet* globalSet = sGlobalsSet.Get ();
	return globalSet ? globalSet->size () : 0;
}

//----------------------------------------------------------------//
ZLContext* ZLContextMgr::Create () {
	
	GlobalsSet* globalSet = sGlobalsSet.Get ();
	
	if ( !globalSet ) {
		globalSet = new GlobalsSet ();
		sGlobalsSet.Set ( globalSet );
	}

	ZLContext* globals = new ZLContext ();
	
	globalSet->insert ( globals );
	sInstance.Set ( globals );
	
	ZLLog::Affirm ();

	return globals;
}

//----------------------------------------------------------------//
void ZLContextMgr::Delete ( ZLContext* globals ) {
		
	GlobalsSet* globalSet = sGlobalsSet.Get ();
	ZLContext* prevInstance = sInstance.Get ();
	
	if ( globalSet ) {
		if ( globalSet->contains ( globals )) {
			
			sInstance.Set ( globals );
			delete globals;
			globalSet->erase ( globals );
			sInstance.Set ( 0 );
		}
		
		// if the global set is empty, delete it
		if ( globalSet->size () == 0 ) {
			delete globalSet;
        	sGlobalsSet.Set ( 0 );
		}
	}
	
	// don't set this to nil until *after* deleting it!
	if ( prevInstance != globals ) {
		sInstance.Set ( prevInstance );
	}
}

//----------------------------------------------------------------//
void ZLContextMgr::Finalize () {

	GlobalsSet* globalSet = sGlobalsSet.Get ();

    if ( globalSet ) {

        GlobalsSetIt globalsIt = globalSet->begin ();
        for ( ; globalsIt != globalSet->end (); ++globalsIt ) {
			ZLContext* instance = *globalsIt;
            sInstance.Set ( instance );
            delete instance;
        }
        
        globalSet->clear ();
        sInstance.Set ( 0 );
        
        delete globalSet;
        sGlobalsSet.Set ( 0 );
    }
}

//----------------------------------------------------------------//
ZLContext* ZLContextMgr::Get () {

	return sInstance.Get ();
}

//----------------------------------------------------------------//
ZLContext* ZLContextMgr::Set ( ZLContext* globals ) {

//	ZLLog_DebugF ( ZLLog::CONSOLE, "ZLContextMgr: setting context %p\n", globals );

	ZLContext* prev = sInstance.Get ();
	sInstance.Set ( globals );
	return prev;
}

//----------------------------------------------------------------//
ZLContextMgr::ZLContextMgr () {
}

//----------------------------------------------------------------//
ZLContextMgr::~ZLContextMgr () {
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

	this->mOriginalContext = ZLContextMgr::Get ();
}

//----------------------------------------------------------------//
ZLScopedContext::~ZLScopedContext () {

	ZLContextMgr::Set ( this->mOriginalContext );
}
