// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <uslscore/USLog.h>

#ifdef ANDROID
	#include <android/log.h>
#endif

//================================================================//
// USLog
//================================================================//

//----------------------------------------------------------------//
void USLog::PrintFile ( FILE* file, cc8* format, ... ) {

	va_list args;
	va_start ( args, format );	
	
	USLog::PrintFile ( file, format, args );
	
	va_end ( args );
}

//----------------------------------------------------------------//
void USLog::PrintFile ( FILE* file, cc8* format, va_list args ) {

	if ( file && file != stdout ) {
		vfprintf ( file, format, args );
		fflush(file);
	}
	else {
		#ifdef ANDROID
			char str [ 1024 ];
			vsnprintf ( str, 1024, format, args );
			__android_log_print(ANDROID_LOG_INFO,"MoaiLog", "%s",str);
		#else
			vprintf ( format, args );
		#endif
	}
}
