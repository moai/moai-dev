// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTSINKVK_H
#define MOAIABSTRACTSNAPSHOTSINKVK_H

#include <moai-gfx-vk/MOAISnapshotVK.h>

//================================================================//
// MOAIAbstractSnapshotSinkVK
//================================================================//
template < typename SNAPSHOT_TYPE >
class MOAIAbstractSnapshotSinkVK {
protected:

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractSnapshotSinkVK_DiscardSnapshot		( SNAPSHOT_TYPE& snapshot ) = 0;

public:
	
	//----------------------------------------------------------------//
	void DiscardSnapshot ( MOAISnapshotVK& snapshot ) {
		this->DiscardSnapshot ( snapshot );
	}

	//----------------------------------------------------------------//
	MOAIAbstractSnapshotSinkVK () {
		static_cast < MOAISnapshotVK < SNAPSHOT_TYPE >* >(( SNAPSHOT_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	~MOAIAbstractSnapshotSinkVK () {
	}
};

#endif
