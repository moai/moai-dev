// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USDIRECTORYITR_H
#define	USDIRECTORYITR_H

#include <uslscore/STLString.h>

//================================================================//
// USDirectoryItr
//================================================================//
class USDirectoryItr {
private:

	ZLDIR*		mItr;
	STLString	mCurrent;

public:
	
	//----------------------------------------------------------------//
	cc8*		Current				();
	void		Finish				();
	cc8*		NextDirectory		();
	cc8*		NextEntry			();
	cc8*		NextFile			();
	void		Start				();
				USDirectoryItr		();
				~USDirectoryItr		();
};

#endif
