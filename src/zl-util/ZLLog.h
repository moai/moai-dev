// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLLOG_H
#define ZLLOG_H

//================================================================//
// ZLLog
//================================================================//
class ZLLog {
public:

	static void* CONSOLE;

	//----------------------------------------------------------------//
	static void	LogF		( void* file, cc8* format, ... );
	static void	LogV		( void* file, cc8* format, va_list args );
};

#endif
