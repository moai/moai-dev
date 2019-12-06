// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLREFCOUNTEDOBJECT_H
#define ZLREFCOUNTEDOBJECT_H

#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLRefCountedObjectBase.h>

class ZLRefCountedObject;
class ZLRefCountedObjectHandle;

//================================================================//
// ZLRefCountedObjectHandle
//================================================================//
class ZLRefCountedObjectHandle :
	public ZLRefCountedObjectBase {
protected:

	ZLRefCountedObject*				mObject;

	//----------------------------------------------------------------//
									ZLRefCountedObjectHandle		();
	
public:

	friend class ZLRefCountedObject;

	//----------------------------------------------------------------//
	ZLRefCountedObject*				GetObject						();
	bool							IsValid							();
									~ZLRefCountedObjectHandle		();

};

//================================================================//
// ZLRefCountedObject
//================================================================//
class ZLRefCountedObject :
	public virtual ZLRefCountedObjectBase {
private:

	friend class ZLAbstractFinalizable_Solo;

	ZLRefCountedObjectHandle*		mHandle;					// only exists if referenced by weak pointer

public:

	friend class ZLRefCountedObjectHandle;
	
	//----------------------------------------------------------------//
	void							Abandon					();
	ZLRefCountedObjectHandle*		GetHandle				();
									ZLRefCountedObject		();
									~ZLRefCountedObject		();
};

#endif
