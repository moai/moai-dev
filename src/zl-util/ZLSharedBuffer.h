// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLSHAREDBUFFER_H
#define ZLSHAREDBUFFER_H

#include <zl-util/ZLRefCountedObjectBase.h>

//================================================================//
// ZLSharedConstBuffer
//================================================================//
class ZLSharedConstBuffer :
	public virtual ZLRefCountedObject {
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
