// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLDIRECTORYITR_H
#define	ZLDIRECTORYITR_H

#include <zl-util/STLString.h>

//================================================================//
// ZLDirectoryItr
//================================================================//
class ZLDirectoryItr {
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
				ZLDirectoryItr		();
				~ZLDirectoryItr		();
};

#endif
