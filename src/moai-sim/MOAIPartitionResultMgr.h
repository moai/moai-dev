// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONRESULTMGR_H
#define	MOAIPARTITIONRESULTMGR_H

#include <moai-sim/MOAIPartitionResultBuffer.h>

class MOAIPartitionResultMgr;

//================================================================//
// MOAIScopedPartitionResultBufferHandle
//================================================================//
class MOAIScopedPartitionResultBufferHandle {
private:

	friend class MOAIPartitionResultMgr;

	MOAIPartitionResultMgr&			mMgr;
	MOAIPartitionResultBuffer*		mBuffer;

	//----------------------------------------------------------------//
			MOAIScopedPartitionResultBufferHandle		( MOAIPartitionResultMgr& mgr );

	//----------------------------------------------------------------//
	inline void operator = ( const MOAIScopedPartitionResultBufferHandle& assign ) {
		this->mBuffer = assign.mBuffer;
		(( MOAIScopedPartitionResultBufferHandle& )assign ).mBuffer = 0;
	}

public:

	//----------------------------------------------------------------//
	MOAIPartitionResultBuffer&		Buffer										();
									~MOAIScopedPartitionResultBufferHandle		();
	
	//----------------------------------------------------------------//
	inline operator MOAIPartitionResultBuffer* () {
		return this->mBuffer;
	}
	
	//----------------------------------------------------------------//
	inline operator MOAIPartitionResultBuffer& () {
		return *this->mBuffer;
	}
};

//================================================================//
// MOAIPartitionResultMgr
//================================================================//
class MOAIPartitionResultMgr :
	public MOAIGlobalClass < MOAIPartitionResultMgr > {
private:
	
	friend class MOAIScopedPartitionResultBufferHandle;
	
	ZLLeanArray < MOAIPartitionResultBuffer* >	mBuffers;
	ZLLeanStack < MOAIPartitionResultBuffer* >	mAvailable;
	
	MOAIPartitionResultBuffer	mBuffer;
	
	//----------------------------------------------------------------//
	void			ReleaseBuffer					( MOAIPartitionResultBuffer* buffer );
	
public:
	
	typedef MOAIScopedPartitionResultBufferHandle Handle;
	
	//----------------------------------------------------------------//
	Handle			GetBufferHandle					();
					MOAIPartitionResultMgr			();
					~MOAIPartitionResultMgr			();
};

#endif
