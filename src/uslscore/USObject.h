// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USOBJECT_H
#define USOBJECT_H

#include <uslscore/USRtti.h>

class USCanary;

//================================================================//
// USObject
//================================================================//
class USObject :
	public RTTIBase {
private:

	USCanary*		mCanary;

	//----------------------------------------------------------------//
			
public:

	friend class USCanary;
	friend class USGcCollector;

	//----------------------------------------------------------------//
	USCanary*		AffirmCanary		();
	u32				GetRefCount			();
	bool			IsInScope			();
	void			Release				();
	void			Retain				();
					USObject			();
	virtual			~USObject			();
};

#endif
