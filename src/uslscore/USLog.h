// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LOG_H
#define LOG_H

//================================================================//
// USLog
//================================================================//
class USLog {

public:
	static FILE* CONSOLE;

	//----------------------------------------------------------------//
	static void	Print		( cc8* format, ... );
	static void	PrintFile	( FILE* file, cc8* format, ... );
};

#endif
