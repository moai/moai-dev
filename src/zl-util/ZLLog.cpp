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

FILE* const ZLLog::CONSOLE = 0;

//----------------------------------------------------------------//
u32 ZLLog::GetLogLevel () {

	return this->mLogLevel;
}

//----------------------------------------------------------------//
bool ZLLog::IsEnabled ( u32 level ) {

	return this->mLogLevel <= level;
}

//----------------------------------------------------------------//
void ZLLog::LogF ( u32 level, FILE* file, cc8* format, ... ) {
	
	if ( this->mLogLevel <= level ) {

		va_list args;
		va_start ( args, format );
		
		ZLLog::LogV ( level, file, format, args );
		
		va_end ( args );
	}
}

//----------------------------------------------------------------//
void ZLLog::LogV ( u32 level, FILE* file, cc8* format, va_list args ) {

	if ( this->mLogLevel <= level ) {
	
		if ( this->mLogFunc ) {
		
			this->mLogFunc ( level, format, args, this->mLogFuncUserdata );
		}
		else {
		
			file = file ? file : ( this->mConsoleRedirect ? this->mConsoleRedirect : CONSOLE );

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

	this->mLogFunc = logFunc;
	this->mLogFuncUserdata = userdata;
}

//----------------------------------------------------------------//
void ZLLog::SetLogLevel ( u32 level ) {

	assert ( level <= LOG_NONE );
	
	this->mLogLevel = level;
}

//----------------------------------------------------------------//
void ZLLog::SetRedirect ( FILE* file ) {

	this->mConsoleRedirect = file;
}

//----------------------------------------------------------------//
ZLLog::ZLLog () :
	mLogLevel ( ZLLog::LOG_DEBUG ),
	mConsoleRedirect ( 0 ),
	mLogFunc ( 0 ),
	mLogFuncUserdata ( 0 ) {
}

//----------------------------------------------------------------//
ZLLog::~ZLLog () {
}
