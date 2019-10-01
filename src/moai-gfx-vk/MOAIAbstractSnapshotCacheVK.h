// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTCACHEVK_H
#define MOAIABSTRACTSNAPSHOTCACHEVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotSignatureVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

class MOAIAbstractSnapshot;

//================================================================//
// MOAIAbstractSnapshotCacheVK
//================================================================//
template < typename SNAPSHOT_TYPE, typename SIGNATURE_TYPE >
class MOAIAbstractSnapshotCacheVK {
protected:

	//----------------------------------------------------------------//
	virtual SNAPSHOT_TYPE*			MOAIAbstractSnapshotCacheVK_GetSnapshot			( const SIGNATURE_TYPE* signature ) = 0;

public:
	
	//----------------------------------------------------------------//
	SNAPSHOT_TYPE* GetSnapshot ( const SIGNATURE_TYPE* signature ) {
		return this->MOAIAbstractSnapshotCacheVK_GetSnapshot ( signature );
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractSnapshotCacheVK () {
		static_cast < MOAIAbstractSnapshotSignatureVK < SIGNATURE_TYPE >* >(( SIGNATURE_TYPE* )NULL );
		static_cast < MOAIAbstractSnapshotVK* >(( SNAPSHOT_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractSnapshotCacheVK () {
	}
};

#endif
