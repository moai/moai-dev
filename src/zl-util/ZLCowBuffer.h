// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCOWBUFFER_H
#define ZLCOWBUFFER_H

#include <zl-util/ZLRefCountedObject.h>

// ZL Copy On Write buffer. Moo!

//================================================================//
// ZLCowBufferInternal
//================================================================//
class ZLCowBufferInternal :
	public ZLRefCountedObject {
private:

	friend class ZLCowBuffer;

	void*				mBuffer;
	size_t				mSize;

	//----------------------------------------------------------------//
						ZLCowBufferInternal		();
						~ZLCowBufferInternal	();
};

//================================================================//
// ZLCowBuffer
//================================================================//
class ZLCowBuffer {
private:

	friend class ZLRevBufferStream;

	ZLCowBufferInternal* mInternal;

	//----------------------------------------------------------------//
	void				AffirmInternal			();

public:

	//----------------------------------------------------------------//
	void*				Alloc					( size_t size );
	void*				Alloc					( size_t size, u8 fill );
	void*				Alloc					( size_t size, const void* fill );
	void				Assign					( const ZLCowBuffer& assign );
	void				Free					();
	const void*			GetBuffer				() const;
	void*				GetBufferMutable		();
	size_t				GetSize					() const;
						ZLCowBuffer				();
						ZLCowBuffer				( size_t size );
						ZLCowBuffer				( size_t size, u8 fill );
						ZLCowBuffer				( size_t size, const void* fill );
						ZLCowBuffer				( const ZLCowBuffer& assign );
						~ZLCowBuffer			();
	
	//----------------------------------------------------------------//
	inline ZLCowBuffer& operator= ( const ZLCowBuffer& rhs ) {
		this->Assign ( rhs );
		return *this;
	}
};

#endif
