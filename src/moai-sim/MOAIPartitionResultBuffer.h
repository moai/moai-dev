// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONRESULTBUFFER_H
#define	MOAIPARTITIONRESULTBUFFER_H

class MOAIProp;

//================================================================//
// MOAIPartitionResult
//================================================================//
class MOAIPartitionResult :
	public ZLRadixKey32Base {
public:

	MOAIProp*	mProp;
	int			mSubPrimID;
	s32			mPriority;
	
	ZLVec3D		mLoc;
	ZLBox		mBounds;
};

//================================================================//
// MOAIPartitionResultBuffer
//================================================================//
/**	@lua	MOAIPartitionResultBuffer
	@text	Class for optimizing spatial queries against sets of primitives.
			Configure for performance; default behavior is a simple list.
*/
class MOAIPartitionResultBuffer {
private:

	friend class MOAIPartition;
	friend class MOAIPartitionCell;

	static const u32 BLOCK_SIZE = 512;

	ZLLeanArray < MOAIPartitionResult >		mBufferA;
	ZLLeanArray < MOAIPartitionResult >		mBufferB;
	
	MOAIPartitionResult*					mResults;
	
	ZLLeanArray < MOAIPartitionResult >*	mMainBuffer;
	ZLLeanArray < MOAIPartitionResult >*	mSwapBuffer;
	
	u32										mTotalResults;

	//----------------------------------------------------------------//
	MOAIPartitionResult*	AffirmSwapBuffer				();
	void					SetResultsBuffer				( MOAIPartitionResult* buffer );
	u32						SortResultsIso					();
	u32						SortResultsLinear				();
	
public:

	static const u32 SORT_FLAG_DESCENDING		= 0x80000000;
	static const u32 SORT_MODE_MASK				= 0x7fffffff;

	enum {
		SORT_NONE,
		
		SORT_ISO,
		
		SORT_DIST_SQUARED_ASCENDING,
		SORT_KEY_ASCENDING,
		SORT_PRIORITY_ASCENDING,
		SORT_X_ASCENDING,
		SORT_Y_ASCENDING,
		SORT_Z_ASCENDING,
		SORT_VECTOR_ASCENDING,
		
		SORT_DIST_SQUARED_DESCENDING		= SORT_DIST_SQUARED_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_KEY_DESCENDING					= SORT_KEY_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_PRIORITY_DESCENDING			= SORT_PRIORITY_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_X_DESCENDING					= SORT_X_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_Y_DESCENDING					= SORT_Y_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_Z_DESCENDING					= SORT_Z_ASCENDING | SORT_FLAG_DESCENDING,
		SORT_VECTOR_DESCENDING				= SORT_VECTOR_ASCENDING | SORT_FLAG_DESCENDING,
	};

	GET ( u32, TotalResults, mTotalResults )
	
	//----------------------------------------------------------------//
	void					Clear							();
	MOAIProp*				FindBest						();
	void					GenerateKeys					( u32 mode, float xScale, float yScale, float zScale, float priority );
							MOAIPartitionResultBuffer		();
							~MOAIPartitionResultBuffer		();
	MOAIPartitionResult*	PopResult						();
	void					Project							( const ZLMatrix4x4& mtx );
	void					PushProps						( lua_State* L );
	void					PushResult						( MOAIProp& prop, u32 key, int subPrimID, s32 priority, const ZLVec3D& loc, const ZLBox& bounds );
	void					Reset							();
	u32						Sort							( u32 mode );
	void					Transform						( const ZLMatrix4x4& mtx, bool transformBounds );
	
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
