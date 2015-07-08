// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLREFCOUNTEDOBJECT_H
#define ZLREFCOUNTEDOBJECT_H

#include <zl-util/ZLAccessors.h>

//================================================================//
// ZLRefCountedObject
//================================================================//
class ZLRefCountedObject {
private:

	u32					mRefCount;

public:

	GET ( u32, RefCount, mRefCount )

	//----------------------------------------------------------------//
	virtual void		Release						();
	virtual void		Retain						();
						ZLRefCountedObject			();
	virtual				~ZLRefCountedObject			();

};

#endif
