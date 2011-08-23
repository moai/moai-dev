// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LOG_H
#define LOG_H

//================================================================//
// USLog
//================================================================//
namespace USLog {

	static FILE* CONSOLE = 0;

	//----------------------------------------------------------------//
	void	Print	( cc8* format, ... );
	void	Print	( FILE* file, cc8* format, ... );
}

#endif
