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

protected:

	//----------------------------------------------------------------//
	virtual void		Delete						();

public:

	GET ( u32, RefCount, mRefCount )

	//----------------------------------------------------------------//
	virtual void		Release						();
	virtual void		Retain						();
						ZLRefCountedObject			();
	virtual				~ZLRefCountedObject			();
};

//================================================================//
// ZLSharedConstBuffer
//================================================================//
class ZLSharedConstBuffer :
	public ZLRefCountedObject {
public:

	virtual const void*		GetConstData			() const = 0;
	virtual size_t			GetSize					() = 0;
	
	//----------------------------------------------------------------//
	static inline const void* GetConstData ( const ZLSharedConstBuffer* buffer ) {
	
		return buffer ? buffer->GetConstData () : 0;
	}
};

//================================================================//
// ZLSharedMutableBuffer
//================================================================//
class ZLSharedMutableBuffer :
	public ZLSharedConstBuffer {
public:

	virtual void*			GetMutableData			() = 0;
	
	//----------------------------------------------------------------//
	static inline void* GetMutableData ( ZLSharedMutableBuffer* buffer ) {
	
		return buffer ? buffer->GetMutableData () : 0;
	}
};

#endif
