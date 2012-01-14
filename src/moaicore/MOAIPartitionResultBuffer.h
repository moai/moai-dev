// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONRESULTBUFFER_H
#define	MOAIPARTITIONRESULTBUFFER_H

class MOAIProp;

//================================================================//
// MOAIPartitionResult
//================================================================//
class MOAIPartitionResult :
	public USRadixKey32Base {
public:

	MOAIProp*	mProp;
	int			mSubPrimID;
	s32			mPriority;
	
	float		mX;
	float		mY;
	float		mZ;
};

//================================================================//
// MOAIPartitionResultBuffer
//================================================================//
/**	@name	MOAIPartitionResultBuffer
	@text	Class for optimizing spatial queries against sets of primitives.
			Configure for performance; default behavior is a simple list.
*/
class MOAIPartitionResultBuffer {
private:

	friend class MOAIPartition;
	friend class MOAIPartitionCell;

	static const u32 BLOCK_SIZE = 512;

	USLeanArray < MOAIPartitionResult >		mMainBuffer;
	USLeanArray < MOAIPartitionResult >		mSwapBuffer;
	MOAIPartitionResult*					mResults;
	u32										mTotalResults;
	
	USLeanArray < MOAIProp* >				mProps;
	u32										mTotalProps;
	
public:

	static const u32 SORT_FLAG_DESCENDING		= 0x80000000;
	static const u32 SORT_MODE_MASK				= 0x7fffffff;

	enum {
		SORT_NONE,
		
		SORT_PRIORITY_ASCENDING,
		SORT_X_ASCENDING,
		SORT_Y_ASCENDING,
		SORT_Z_ASCENDING,
		SORT_VECTOR_ASCENDING,
		
		SORT_PRIORITY_DESCENDING	= SORT_PRIORITY_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_X_DESCENDING			= SORT_X_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_Y_DESCENDING			= SORT_Y_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_Z_DESCENDING			= SORT_Z_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_VECTOR_DESCENDING		= SORT_VECTOR_ASCENDING | SORT_FLAG_DESCENDING,
	};

	GET ( u32, TotalResults, mTotalResults )
	
	//----------------------------------------------------------------//
	void					Clear							();
	MOAIProp*				FindBest						( u32 mode, float xScale, float yScale, float zScale, float priority );
							MOAIPartitionResultBuffer		();
							~MOAIPartitionResultBuffer		();
	MOAIPartitionResult*	PopResult						();
	u32						PrepareResults					( u32 mode );
	u32						PrepareResults					( u32 mode, bool expand, float xScale, float yScale, float zScale, float priority );
	void					PushProp						( MOAIProp& prop );
	void					PushResult						( MOAIProp& prop, int subPrimID, s32 priority, float x, float y, float z );
	void					PushResultProps					( lua_State* L );
	void					Reset							();
	
	//----------------------------------------------------------------//
	inline MOAIPartitionResult* GetResult ( u32 idx ) {

		if ( this->mResults && ( idx < this->mTotalResults )) {
			return &this->mResults [ idx ];
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	inline MOAIPartitionResult* GetResultUnsafe ( u32 idx ) {
		
		return &this->mResults [ idx ];
	}
};

#endif
