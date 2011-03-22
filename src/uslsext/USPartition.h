// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USPARTITION_H
#define	USPARTITION_H

#include <uslsext/USBox.h>
#include <uslsext/USFrustum.h>
#include <uslsext/USGrid.h>

class USPartitionBase;
class USPartitionCell;
class USPartitionLayer;

//================================================================//
// USPartitionPrimBase
//================================================================//
class USPartitionPrimBase {
	friend class USPartitionBase;
	friend class USPartitionCell;
	friend class USPartitionLayer;
protected:
	
	void*								mData;
	
	USPartitionLayer*					mLayer;
	USPartitionCell*					mCell;
	
	USLeanLink < USPartitionPrimBase* >	mLinkInCell;

	u32				mQueryMask;
	USRect			mWorldBounds;
	float			mCellSize;

	//----------------------------------------------------------------//
	virtual USPartitionBase*	GetPartitionBase		() = 0;
	void						PlaceInPartition		();

public:

	GET_SET ( u32, QueryMask, mQueryMask )

	//----------------------------------------------------------------//
	bool			GetCellRect					( USRect* cellRect, USRect* paddedRect = 0 );
	USRect			GetWorldBounds				();
	bool			InCell						();
	void			SetWorldBounds				( const USRect& bounds );
	void			SetWorldBoundsEmpty			();
	void			SetWorldBoundsGlobal		();
					USPartitionPrimBase			();
	virtual			~USPartitionPrimBase		();
};

//================================================================//
// USPartitionCell
//================================================================//
class USPartitionCell {
	friend class USPartitionBase;
	friend class USPartitionLayer;
	friend class USPartitionPrimBase;
private:
	
	typedef USLeanList < USPartitionPrimBase* >::Iterator PrimIt;
	USLeanList < USPartitionPrimBase* > mPrims;

	//----------------------------------------------------------------//
	void			GatherPrims				( USPartitionBase& partition, USPartitionPrimBase* ignore, u32 mask );
	void			GatherPrims				( USPartitionBase& partition, USPartitionPrimBase* ignore, USVec2D& point, u32 mask );
	void			GatherPrims				( USPartitionBase& partition, USPartitionPrimBase* ignore, USRect& rect, u32 mask );
	void			InsertPrim				( USPartitionPrimBase& prim );
	void			RemovePrim				( USPartitionPrimBase& prim );

public:

	//----------------------------------------------------------------//
					USPartitionCell		();
					~USPartitionCell	();
};

//================================================================//
// USPartitionLayer
//================================================================//
class USPartitionLayer {
	friend class USPartitionBase;
	friend class USPartitionPrimBase;
private:
	
	USGridSpace								mGridSpace;
	USLeanArray < USPartitionCell >			mCells;
	float									mCellSize;

	//----------------------------------------------------------------//
	void					GatherPrims			( USPartitionBase& partition, USPartitionPrimBase* ignore, u32 mask );
	void					GatherPrims			( USPartitionBase& partition, USPartitionPrimBase* ignore, USVec2D point, u32 mask );
	void					GatherPrims			( USPartitionBase& partition, USPartitionPrimBase* ignore, USRect rect, u32 mask );
	USPartitionCell*		GetCell				( USPartitionPrimBase& prim );
	void					Init				( float cellSize, u32 width, u32 height );
	void					MovePrim			( USPartitionPrimBase& prim );

public:

	//----------------------------------------------------------------//
					USPartitionLayer	();
					~USPartitionLayer	();
};

//================================================================//
// USPartitionBase
//================================================================//
class USPartitionBase {
	friend class USPartitionCell;
	friend class USPartitionLayer;
	friend class USPartitionPrimBase;
protected:

	enum {
		CHUNK_SIZE = 256,
	};

	USLeanArray < USPartitionLayer >	mLayers;
	USPartitionCell						mEmpties;
	USPartitionCell						mGlobals;

	u32									mTotalPrims;

	USLeanArray < void* >				mResults;
	u32									mTotalResults;

	//----------------------------------------------------------------//
	void					Grow				();
	void					InsertPrim			( USPartitionPrimBase& prim, void* data );
	void					MakeGlobal			( USPartitionPrimBase& prim );
	void					MovePrim			( USPartitionPrimBase& prim );
	void					PushResult			( void* result );
	void					RemovePrim			( USPartitionPrimBase& prim );
	void					ResetResults		();

public:

	GET ( u32, TotalResults, mTotalResults )

	//----------------------------------------------------------------//
	u32			GatherPrims			( USPartitionPrimBase* ignore, u32 mask = 0xffffffff );
	u32			GatherPrims			( USVec2D& point, USPartitionPrimBase* ignore, u32 mask = 0xffffffff );
	u32			GatherPrims			( USRect& rect, USPartitionPrimBase* ignore, u32 mask = 0xffffffff );
	void		ReserveLayers		( int totalLayers );
	void		SetLayer			( int layerID, float cellSize, int width, int height );
				USPartitionBase		();
	virtual		~USPartitionBase	();
};

//================================================================//
// USPartitionBase
//================================================================//
template < typename TYPE >
class USPartition :
	public USPartitionBase {
public:

	//----------------------------------------------------------------//
	inline TYPE* GetResult ( u32 i ) {
		return ( TYPE* )this->mResults [ i ];
	}

	//----------------------------------------------------------------//
	virtual void InsertPrim ( TYPE& prim ) {
	
		this->USPartitionBase::InsertPrim ( prim, &prim );
		prim.SetPartition ( this );
		
		prim.mCellSize = -1.0f;
		prim.PlaceInPartition ();
	}

	//----------------------------------------------------------------//
	virtual void RemovePrim ( TYPE& prim ) {
		this->USPartitionBase::RemovePrim ( prim );
		prim.SetPartition ( 0 );
	}	
};

//================================================================//
// USPartitionPrim
//================================================================//
template < typename TYPE >
class USPartitionPrim :
	public USPartitionPrimBase {
private:

	friend class USPartition < TYPE >;

	USPartition < TYPE >* mPartition;

	//----------------------------------------------------------------//
	USPartitionBase* GetPartitionBase () {
		return this->mPartition;
	}

	//----------------------------------------------------------------//
	void SetPartition ( USPartition < TYPE >* partition ) {
		this->mPartition = partition;
	}

public:

	//----------------------------------------------------------------//
	USPartition < TYPE >* GetPartition () {
		return this->mPartition;
	}

	//----------------------------------------------------------------//
	USPartitionPrim () :
		mPartition ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~USPartitionPrim () {
	}
};

#endif
