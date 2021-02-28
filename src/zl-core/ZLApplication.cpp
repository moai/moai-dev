// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-core/ZLApplication.h>
#include <zl-core/ZLLog.h>

//#define ZL_ENABLE_CONTEXT_DEBUG_LOG

//================================================================//
// ZLApplicationClass
//================================================================//

//----------------------------------------------------------------//
void ZLApplicationGlobalBase::Finalize () {
	this->ZLApplicationGlobalBase_Finalize ();
}

//----------------------------------------------------------------//
void ZLApplicationGlobalBase::Initialize () {
	this->ZLApplicationGlobalBase_Initialize ();
}

//----------------------------------------------------------------//
bool ZLApplicationGlobalBase::IsValid () {
	return ( this->mSelf != NULL );
}

//----------------------------------------------------------------//
ZLApplicationGlobalBase::ZLApplicationGlobalBase () :
	mSelf ( NULL ) {
}

//----------------------------------------------------------------//
ZLApplicationGlobalBase::~ZLApplicationGlobalBase () {
}

//----------------------------------------------------------------//
void ZLApplicationGlobalBase::ZLApplicationGlobalBase_Finalize () {
}

//----------------------------------------------------------------//
void ZLApplicationGlobalBase::ZLApplicationGlobalBase_Initialize () {
}

//================================================================//
// ZLApplication
//================================================================//

//----------------------------------------------------------------//
void ZLApplication::AffirmSize ( ZLSize size ) {

	if ( size < this->mGlobals.Size ()) return;
	this->mGlobals.GrowChunked ( size, CHUNK_SIZE, NULL );
}

//----------------------------------------------------------------//
ZLApplication::ZLApplication () {
}

//----------------------------------------------------------------//
ZLApplication::~ZLApplication () {

	ZLScopedLock lock ( ZLApplication::mMutex );
	ZLLeanArray < ZLApplicationGlobalBase* >& globals = this->mGlobals;

	// get the log object here, since it will be markes as invalid with all the others below
	// no longer safe to use the ZLLog macros
	#ifdef ZL_ENABLE_CONTEXT_DEBUG_LOG
		ZLLog_Debug ( "ZLApplication::~ZLApplication %p\n", this );
	#endif

	size_t total = globals.Size ();
	
	// finalize everything
	for ( size_t i = 1; i <= total; ++i ) {
		ZLApplicationGlobalBase* base = globals [ total - i ];

		if ( base ) {
		
			#ifdef ZL_ENABLE_CONTEXT_DEBUG_LOG
				ZLLog_Debug ( ZLApplication: finalizing global %p\n", base );
			#endif
			
			base->Finalize ();
		}
	}
	
	// mark everything as invalid
	for ( size_t i = 1; i <= total; ++i ) {
		ZLApplicationGlobalBase* base = globals [ total - i ];
	
		if ( base ) {
	
			#ifdef ZL_ENABLE_CONTEXT_DEBUG_LOG
				ZLLog_Debug ( "ZLApplication: invalidating global %p\n", base );
			#endif
			
			base->mSelf = NULL;
		}
	}

	// and officially delete everything
	for ( size_t i = 1; i <= total; ++i ) {
		ZLApplicationGlobalBase* base = globals [ total - i ];

		if ( base ) {
			delete base;
			globals [ total - i ] = NULL;
		}
	}
}
