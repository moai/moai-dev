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

FILE* const			ZLLog::CONSOLE				= 0;
u32					ZLLog::sLogLevel			= ZL_DEFAULT_LOG_LEVEL;
FILE*				ZLLog::sConsoleRedirect		= 0;
ZLLog::LogFunc		ZLLog::sLogFunc				= 0;
void*				ZLLog::sLogFuncUserdata		= 0;

//----------------------------------------------------------------//
u32 ZLLog::GetLogLevel () {

	return sLogLevel;
}

//----------------------------------------------------------------//
bool ZLLog::IsEnabled ( u32 level ) {

	return ( sLogLevel <= level );
}

//----------------------------------------------------------------//
void ZLLog::LogF ( u32 level, FILE* file, cc8* format, ... ) {

	if ( sLogLevel <= level ) {

		va_list args;
		va_start ( args, format );
		
		ZLLog::LogV ( level, file, format, args );
		
		va_end ( args );
	}
}

//----------------------------------------------------------------//
void ZLLog::LogV ( u32 level, FILE* file, cc8* format, va_list args ) {
	
	if ( sLogLevel <= level ) {
	
		if ( sLogFunc ) {
		
			sLogFunc ( format, args, sLogFuncUserdata );
		}
		else {
		
			file = file ? file : ( sConsoleRedirect ? sConsoleRedirect : CONSOLE );

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
void ZLLog::SetLogFunc	( LogFunc logFunc, void* userdata ) {

	sLogFunc = logFunc;
	sLogFuncUserdata = userdata;
}

//----------------------------------------------------------------//
void ZLLog::SetLogLevel ( u32 level ) {

	assert ( level <= LOG_NONE );
	sLogLevel = level;
}

//----------------------------------------------------------------//
void ZLLog::SetRedirect ( FILE* file ) {

	sConsoleRedirect = file;
}
