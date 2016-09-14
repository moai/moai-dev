// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <zl-util/ZLLog.h>

//================================================================//
// ZLLogContext
//================================================================//
	
//----------------------------------------------------------------//
ZLLogContext::ZLLogContext () :
	mLogLevel ( ZLLog::LOG_NONE ),
	mConsoleRedirect ( 0 ),
	mLogFunc ( 0 ),
	mLogFuncUserdata ( 0 ) {
}

//================================================================//
// ZLLog
//================================================================//

FILE* const ZLLog::CONSOLE = 0;

ZLThreadLocalPtr < ZLLogContext > ZLLog::sContext;

//----------------------------------------------------------------//
ZLLogContext* ZLLog::AffirmContext () {

	ZLLogContext* context = sContext.Get ();
	if ( !context ) {
		context = new ( ZLLogContext );
		sContext.Set ( context );
	}
	return context;
}

//----------------------------------------------------------------//
void ZLLog::ClearContext () {

	ZLLogContext* context = sContext.Get ();
	if ( context ) {
		delete ( context );
		sContext.Set ( 0 );
	}
}

//----------------------------------------------------------------//
u32 ZLLog::GetLogLevel () {

	ZLLogContext* context = ZLLog::AffirmContext ();
	return context ? context->mLogLevel : LOG_NONE;
}

//----------------------------------------------------------------//
bool ZLLog::IsEnabled ( u32 level ) {

	ZLLogContext* context = ZLLog::AffirmContext ();
	return context ? ( context->mLogLevel <= level ) : false;
}

//----------------------------------------------------------------//
void ZLLog::LogF ( u32 level, FILE* file, cc8* format, ... ) {

	ZLLogContext* context = ZLLog::AffirmContext ();
	
	if ( context->mLogLevel <= level ) {

		va_list args;
		va_start ( args, format );
		
		ZLLog::LogV ( level, file, format, args );
		
		va_end ( args );
	}
}

//----------------------------------------------------------------//
void ZLLog::LogV ( u32 level, FILE* file, cc8* format, va_list args ) {
	
	ZLLogContext* context = ZLLog::AffirmContext ();
	
	if ( context->mLogLevel <= level ) {
	
		if ( context->mLogFunc ) {
		
			context->mLogFunc ( level, format, args, context->mLogFuncUserdata );
		}
		else {
		
			file = file ? file : ( context->mConsoleRedirect ? context->mConsoleRedirect : CONSOLE );

			if ( file ) {
				zl_vfprintf (( FILE* )file, format, args );
			}
			else {
				zl_vprintf ( format, args );
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLLog::SetLogFunc	( ZLLogFunc logFunc, void* userdata ) {

	ZLLogContext* context = ZLLog::AffirmContext ();
	
	if ( context ) {
		context->mLogFunc = logFunc;
		context->mLogFuncUserdata = userdata;
	}
}

//----------------------------------------------------------------//
void ZLLog::SetLogLevel ( u32 level ) {

	assert ( level <= LOG_NONE );
	
	ZLLogContext* context = ZLLog::AffirmContext ();
	if ( context ) {
		context->mLogLevel = level;
	}
}

//----------------------------------------------------------------//
void ZLLog::SetRedirect ( FILE* file ) {

	ZLLogContext* context = ZLLog::AffirmContext ();
	if ( context ) {
		context->mConsoleRedirect = file;
	}
}
