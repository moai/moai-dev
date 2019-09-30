// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTCACHEVK_H
#define MOAIABSTRACTSNAPSHOTCACHEVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotSignatureVK.h>
#include <moai-gfx-vk/MOAISnapshotVK.h>

class MOAIAbstractSnapshot;

//================================================================//
// MOAIAbstractSnapshotCacheVK
//================================================================//
template < typename SNAPSHOT_TYPE, typename SIGNATURE_TYPE >
class MOAIAbstractSnapshotCacheVK {
protected:

	//----------------------------------------------------------------//
	virtual void								MOAIAbstractSnapshotCacheVK_CacheSnapshot		( const SIGNATURE_TYPE& signature, ZLStrongPtr < SNAPSHOT_TYPE > snapshot ) = 0;
	virtual ZLStrongPtr < SNAPSHOT_TYPE >		MOAIAbstractSnapshotCacheVK_GetSnapshot			( const SIGNATURE_TYPE& signature ) = 0;

public:
	
	//----------------------------------------------------------------//
	void CacheSnapshot ( const SIGNATURE_TYPE& signature, ZLStrongPtr < SNAPSHOT_TYPE > snapshot ) {
		this->MOAIAbstractSnapshotCacheVK_CacheSnapshot ( signature, snapshot );
	}
	
	//----------------------------------------------------------------//
	ZLStrongPtr < SNAPSHOT_TYPE > GetSnapshot ( const SIGNATURE_TYPE& signature ) {
		return this->MOAIAbstractSnapshotCacheVK_GetSnapshot ( signature );
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractSnapshotCacheVK () {
		static_cast < MOAISnapshotVK < SNAPSHOT_TYPE >* >(( SNAPSHOT_TYPE* )NULL );
		static_cast < MOAIAbstractSnapshotSignatureVK < SIGNATURE_TYPE >* >(( SIGNATURE_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	~MOAIAbstractSnapshotCacheVK () {
	}
};

#endif
