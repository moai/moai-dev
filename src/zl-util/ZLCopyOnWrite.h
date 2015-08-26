// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCOPYONWRITE_H
#define ZLCOPYONWRITE_H

#include <zl-util/ZLRefCountedObject.h>

//================================================================//
// ZLCopyOnWriteBuffer
//================================================================//
class ZLCopyOnWriteBuffer :
	public ZLRefCountedObject {
private:

	friend class ZLCopyOnWrite;

	void*				mBuffer;
	size_t				mSize;

	//----------------------------------------------------------------//
						ZLCopyOnWriteBuffer		();
						~ZLCopyOnWriteBuffer	();
};

//================================================================//
// ZLCopyOnWrite
//================================================================//
class ZLCopyOnWrite {
private:

	friend class ZLRevBufferStream;

	ZLCopyOnWriteBuffer* mInternal;

	//----------------------------------------------------------------//
	void				AffirmInternal			();

public:

	//----------------------------------------------------------------//
	void*				Alloc					( size_t size );
	void*				Alloc					( size_t size, u8 fill );
	void*				Alloc					( size_t size, const void* fill );
	void				Assign					( const ZLCopyOnWrite& assign );
	void				Free					();
	const void*			GetBuffer				() const;
	void*				GetBufferMutable		();
	size_t				GetSize					() const;
						ZLCopyOnWrite			();
						ZLCopyOnWrite			( size_t size );
						ZLCopyOnWrite			( size_t size, u8 fill );
						ZLCopyOnWrite			( size_t size, const void* fill );
						ZLCopyOnWrite			( const ZLCopyOnWrite& assign );
						~ZLCopyOnWrite			();
	
	//----------------------------------------------------------------//
	inline ZLCopyOnWrite& operator= ( const ZLCopyOnWrite& rhs ) {
		this->Assign ( rhs );
		return *this;
	}
};

#endif
