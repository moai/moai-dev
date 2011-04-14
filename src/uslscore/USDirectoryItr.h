// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	DIRECTORYITR_H
#define	DIRECTORYITR_H

#include <uslscore/USFilename.h>
#include <uslscore/STLString.h>

//================================================================//
// USDirectoryItr
//================================================================//
class USDirectoryItr {
private:

	USFilename	mPath;
	void*		mSearchHandle;
	void*		mCurrentEntry;
	s32			mSearchState;
	STLString	mCurrentName;

public:
	
	//----------------------------------------------------------------//
	cc8*		Current			();
	void		Finish			();
	cc8*		NextDirectory	();
	cc8*		NextFile		();
	void		Start			();
				USDirectoryItr	();
				~USDirectoryItr	();
};

#endif
