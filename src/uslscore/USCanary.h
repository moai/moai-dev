// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USCANARY_H
#define USCANARY_H

#include <uslscore/STLString.h>
#include <uslscore/USFactory.h>

class USGcLink;
class USObject;

//================================================================//
// USCanary
//================================================================//
class USCanary {
protected:

	u32				mRefCount;
	u32				mStrongRefs;
	USObject*		mObject;

	//----------------------------------------------------------------//	
					USCanary		();
	
public:

	friend class USObject;

	//----------------------------------------------------------------//
	void			ForceDelete		();
	USObject*		GetObject		();
	bool			IsValid			();
	void			Release			( bool strong );
	void			Retain			( bool strong );
					~USCanary		();
};

#endif
