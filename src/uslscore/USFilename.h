// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	FILENAME_H
#define	FILENAME_H

#include <uslscore/STLString.h>
#include <uslscore/USAccessors.h>

//================================================================//
// USFilename
//================================================================//
class USFilename {
private:
	
	char mBuffer [ PATH_MAX ];

public:

	friend class USFileSys;

	GET ( cc8*, Str, mBuffer );

	//----------------------------------------------------------------//
	char*		GetAbsoluteDirPath		( cc8* path );
	char*		GetAbsoluteFilePath		( cc8* path );
	char*		GetCurrentPath			();
	char*		GetRelativePath			( cc8* path );
	char*		TruncateFilename		( cc8* filename );
				USFilename				();
				~USFilename				();
};

#endif
