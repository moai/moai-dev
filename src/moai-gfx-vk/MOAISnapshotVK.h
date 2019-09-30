// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISNAPSHOTVK_H
#define MOAISNAPSHOTVK_H

#include <moai-gfx-vk/MOAIAbstractCommandBufferMemberVK.h>

template < typename SNAPSHOT_TYPE > class MOAIAbstractSnapshotSinkVK;

//================================================================//
// MOAISnapshotVK
//================================================================//
template < typename SNAPSHOT_TYPE >
class MOAISnapshotVK :
	public virtual MOAIAbstractCommandBufferMemberVK {
private:

	MOAIAbstractSnapshotSinkVK < SNAPSHOT_TYPE >* mSnapshotSink;
	
	//----------------------------------------------------------------//
	void MOAIAbstractCommandBufferMemberVK_Discard () {
		if ( this->mSnapshotSink ) {
			this->mSnapshotSink->DiscardSnapshot ( *this );
		}
	}

public:
	
	//----------------------------------------------------------------//
	MOAISnapshotVK () :
		mSnapshotSink ( NULL ) {
		static_cast < MOAISnapshotVK < SNAPSHOT_TYPE >* >(( SNAPSHOT_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	~MOAISnapshotVK () {
	}
	
	//----------------------------------------------------------------//
	void SetSnapshotSink ( MOAIAbstractSnapshotSinkVK < SNAPSHOT_TYPE >& snapshotSink ) {
		this->mSnapshotSink = &snapshotSink;
	}
};

#endif
