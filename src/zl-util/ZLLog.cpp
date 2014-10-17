// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <zl-util/ZLLog.h>

//================================================================//
// ZLLog
//================================================================//

FILE*			ZLLog::CONSOLE				= 0;
ZLLog::LogFunc	ZLLog::sLogFunc				= 0;
void*			ZLLog::sLogFuncUserdata		= 0;

//----------------------------------------------------------------//
void ZLLog::LogF ( FILE* file, cc8* format, ... ) {

	va_list args;
	va_start ( args, format );	
	
	ZLLog::LogV ( file, format, args );
	
	va_end ( args );
}

//----------------------------------------------------------------//
void ZLLog::LogV ( FILE* file, cc8* format, va_list args ) {
	
	if ( sLogFunc ) {
	
		sLogFunc ( file, format, args, sLogFuncUserdata );
	}
	else {
	
		if ( file ) {
			zl_vfprintf (( FILE* )file, format, args );
		}
		else {
			zl_vprintf ( format, args );
		}
	}
}

//----------------------------------------------------------------//
void ZLLog::SetLogFunc	( LogFunc logFunc, void* userdata ) {

	sLogFunc = logFunc;
	sLogFuncUserdata = userdata;
}
