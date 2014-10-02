// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLLOG_H
#define ZLLOG_H

//================================================================//
// ZLLog
//================================================================//
class ZLLog {
public:

	static FILE* CONSOLE;

	// TODO: some danger here if the caller isn't using VFS; should change this to
	// not take a file pointer. implementers will need to provide their own.
	typedef void ( *LogFunc )( void* file, cc8* format, va_list args, void* userdata );
	static LogFunc sLogFunc;
	
	static void* sLogFuncUserdata;

	//----------------------------------------------------------------//
	static void	LogF		( FILE* file, cc8* format, ... );
	static void	LogV		( FILE* file, cc8* format, va_list args );
	static void SetLogFunc	( LogFunc logFunc, void* userdata );
};

#endif
