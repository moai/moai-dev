// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-core/ZLContext.h>
#include <zl-core/ZLLog.h>

//#define ZL_ENABLE_CONTEXT_DEBUG_LOG

//================================================================//
// ZLHasContext
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
TYPE& ZLHasContext::Get () {
	assert ( this->mContext );
	return this->mContext->Get < TYPE >();
}

//----------------------------------------------------------------//
void ZLHasContext::InitializeContext ( ZLContext* context ) {
	assert ( context );
	assert (( this->mContext == NULL ) || ( this->mContext == context ));
	this->mContext = context;
}

//----------------------------------------------------------------//
ZLHasContext::ZLHasContext () :
	mContext ( NULL ) {
}

//----------------------------------------------------------------//
ZLHasContext::~ZLHasContext () {
}

//================================================================//
// ZLContextClass
//================================================================//

//----------------------------------------------------------------//
void ZLContextClass::Finalize () {
	this->ZLContextClass_Finalize ();
}

//----------------------------------------------------------------//
void ZLContextClass::Initialize () {
	this->ZLContextClass_Initialize ();
}

//----------------------------------------------------------------//
bool ZLContextClass::IsValid () {
	return this->mContext != NULL;
}

//----------------------------------------------------------------//
ZLContextClass::ZLContextClass () {
}

//----------------------------------------------------------------//
ZLContextClass::~ZLContextClass () {
}

//----------------------------------------------------------------//
void ZLContextClass::ZLContextClass_Finalize () {
}

//----------------------------------------------------------------//
void ZLContextClass::ZLContextClass_Initialize () {
}

//================================================================//
// ZLContext
//================================================================//

//----------------------------------------------------------------//
void ZLContext::AffirmSize ( ZLSize size ) {

	if ( size < this->mGlobals.Size ()) return;
	this->mGlobals.GrowChunked ( size, CHUNK_SIZE );
	
	size = this->mGlobals.Size ();
	for ( ZLIndex i = 0; i < size; ++i ) {
	
		ZLContextPair& pair = this->mGlobals [ i ];
		if ( pair.mAliasOf ) {
			assert ( pair.mAliasID < this->mGlobals.Size ());
			pair.mAliasOf = &this->mGlobals [ pair.mAliasID ];
		}
	}
}

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
	
	// finalize everything
	for ( size_t i = 1; i <= total; ++i ) {
		ZLContextPair& pair = this->mGlobals [ total - i ];
		ZLContextClass* global = pair.mGlobalBase;

		if ( global && ( pair.mAliasOf == NULL )) {
		
			#ifdef ZL_ENABLE_CONTEXT_DEBUG_LOG
				log.LogF ( ZLLog::LOG_DEBUG, ZLLog::CONSOLE, "ZLContext: finalizing global %p\n", global );
			#endif
			
			global->Finalize ();
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
		ZLContextClass* global = pair.mGlobalBase;

		if ( global && ( pair.mAliasOf == NULL )) {
		
			#ifdef ZL_ENABLE_CONTEXT_DEBUG_LOG
				log.LogF ( ZLLog::LOG_DEBUG, ZLLog::CONSOLE, "ZLContext: deleting global %p\n", global );
			#endif
			
			delete global;
		}
	}
}
