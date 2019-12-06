// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTRANSMIGRATIONCACHE_H
#define	ZLTRANSMIGRATIONCACHE_H

#include <zl-util/STLMap.h>
#include <zl-util/STLSet.h>

class ZLAbstractTransmigrationCache;

//================================================================//
// ZLAbstractTransmigrationMemoMap
//================================================================//
class ZLAbstractTransmigrationMemoMap {
private:

	friend class ZLTransmigrationCache;

	bool	mIsActive;

	//----------------------------------------------------------------//
	virtual void		ZLAbstractTransmigrationMemoMap_Clear		() = 0;

	//----------------------------------------------------------------//
	void Clear () {
		this->ZLAbstractTransmigrationMemoMap_Clear ();
		this->mIsActive = false;
	}

public:

	//----------------------------------------------------------------//
	ZLAbstractTransmigrationMemoMap () :
		mIsActive ( false ) {
	}
	
	//----------------------------------------------------------------//
	virtual ~ZLAbstractTransmigrationMemoMap () {
	}
};

//================================================================//
// ZLTransmigrationMemoMap
//================================================================//
template < typename MEMO_TYPE >
class ZLTransmigrationMemoMap :
	public ZLAbstractTransmigrationMemoMap,
	public STLMap < const void*, MEMO_TYPE > {
private:

	friend class ZLTransmigrationCache;

	//----------------------------------------------------------------//
	void ZLAbstractTransmigrationMemoMap_Clear () {
		this->clear ();
	}

public:

	//----------------------------------------------------------------//
	ZLTransmigrationMemoMap () {
	}
	
	//----------------------------------------------------------------//
	virtual ~ZLTransmigrationMemoMap () {
	}
};

//================================================================//
// ZLTransmigrationCache
//================================================================//
class ZLTransmigrationCache {
private:

	friend class ZLAbstractFinalizable_Solo;

	ZLSize												mActiveCount;
	STLSet < ZLAbstractTransmigrationMemoMap* >			mCleanupSet;
	
	//----------------------------------------------------------------//
	void			BeginTransmigration					();
	void			EndTransmigration					();

public:

	//----------------------------------------------------------------//
	static ZLTransmigrationCache&		Get									();
	bool								IsActive							() const;
										ZLTransmigrationCache				();
	
	//----------------------------------------------------------------//
	template < typename MEMO_TYPE >
	const MEMO_TYPE& Memo ( const void* addr ) const {
	
		ZLTransmigrationMemoMap < MEMO_TYPE >& memoMap = ZLThreadLocalPtr < ZLTransmigrationMemoMap < MEMO_TYPE > >::Singleton ();
		
		typename ZLTransmigrationMemoMap < MEMO_TYPE >::const_iterator mapIt = memoMap.find ( addr );
		assert ( mapIt != memoMap.cend ());
		return mapIt->second;
	}
	
	//----------------------------------------------------------------//
	template < typename MEMO_TYPE >
	void Memo ( const void* addr, const MEMO_TYPE& memo ) {
	
		ZLTransmigrationMemoMap < MEMO_TYPE >& memoMap = ZLThreadLocalPtr < ZLTransmigrationMemoMap < MEMO_TYPE > >::Singleton ();
		if ( !memoMap.mIsActive ) {
			this->mCleanupSet.insert ( &memoMap );
			memoMap.mIsActive = true;
		}
		memoMap [ addr ] = memo;
	}
};

#endif
