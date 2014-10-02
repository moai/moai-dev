// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <zl-util/ZLLog.h>

#ifdef ANDROID
	#include <android/log.h>
#endif

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
			vfprintf (( FILE* )file, format, args );
		}
		else {
			#ifdef ANDROID
				__android_log_vprint ( ANDROID_LOG_INFO, "MoaiLog", format, args );
			#else
				vprintf ( format, args );
			#endif
		}
	}
}

//----------------------------------------------------------------//
void ZLLog::SetLogFunc	( LogFunc logFunc, void* userdata ) {

	sLogFunc = logFunc;
	sLogFuncUserdata = userdata;
}
