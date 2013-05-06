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

	//----------------------------------------------------------------//
	static void	Print		( cc8* format, ... );
	static void	PrintFile	( FILE* file, cc8* format, ... );
	static void	PrintFileV	( FILE* file, cc8* format, va_list args );
};

#endif
