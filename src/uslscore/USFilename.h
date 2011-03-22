// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	FILENAME_H
#define	FILENAME_H

#include <uslscore/STLString.h>

//================================================================//
// USFilename
//================================================================//
class USFilename {
private:

	//----------------------------------------------------------------//
	char*		Normalize			( cc8* path );

public:

	int mLength;
	int mNameIdx;
	int mExtIdx;
	char mBuffer [ PATH_MAX ];

	//----------------------------------------------------------------//
	void		Append				( char c );
	void		Append				( cc8* str );
	char*		Bless				( cc8* path );
	void		Clear				();
	u32			ComparePath			( USFilename& filename );
	char*		ConvertToPath		();
	void		Copy				( USFilename& filename );
	char*		Expand				( cc8* filename );
	char*		ExpandPath			( cc8* filename );
	char*		GetCurrentPath		();
	char*		GetExt				( cc8* filename );
	char*		GetName				( cc8* filename );
	char*		GetNameNoExt		( cc8* filename );
	char*		GetPath				( cc8* filename );
	char*		GetRelativePath		( cc8* path );
				USFilename			();
				~USFilename			();
};

//================================================================//
// USPathOps
//================================================================//
namespace USPathOps {

	//----------------------------------------------------------------//
	char		Bless				( char c );
	STLString	Bless				( cc8* path );
	u32			ComparePaths		( cc8* p0, cc8* p1 );
	STLString	GetExt				( cc8* filename );
	STLString	GetName				( cc8* filename );
	STLString	GetNameNoExt		( cc8* filename );
	STLString	GetPath				( cc8* filename, bool expand = false );
	bool		IsSeparator			( const char c );
}

#endif
