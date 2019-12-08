// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTFACTORYVK_H
#define MOAIABSTRACTSNAPSHOTFACTORYVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAICommandBufferVK.h>

//================================================================//
// MOAIAbstractSnapshotFactoryVK
//================================================================//
template < typename SNAPSHOT_TYPE >
class MOAIAbstractSnapshotFactoryVK {
protected:

	//----------------------------------------------------------------//
	virtual SNAPSHOT_TYPE*		MOAIAbstractSnapshotFactoryVK_GetSnapshot		( MOAICommandBufferVK& commandBuffer ) = 0;

public:
	
	//----------------------------------------------------------------//
	SNAPSHOT_TYPE* GetSnapshot ( MOAICommandBufferVK& commandBuffer ) {
		SNAPSHOT_TYPE* snapshot = this->MOAIAbstractSnapshotFactoryVK_GetSnapshot ( commandBuffer );
		commandBuffer.Pin ( *snapshot );
		return snapshot;
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractSnapshotFactoryVK () {
		static_cast < MOAIAbstractSnapshotVK* >(( SNAPSHOT_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractSnapshotFactoryVK () {
	}
};

#endif
