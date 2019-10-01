// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTSUBJECTVK_H
#define MOAIABSTRACTSNAPSHOTSUBJECTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

//================================================================//
// MOAIAbstractSnapshotSubjectVK
//================================================================//
template < typename SNAPSHOT_TYPE >
class MOAIAbstractSnapshotSubjectVK {
protected:

	//----------------------------------------------------------------//
	virtual SNAPSHOT_TYPE*		MOAIAbstractSnapshotSubjectVK_MakeSnapshot			() = 0;

public:
	
	//----------------------------------------------------------------//
	SNAPSHOT_TYPE* MakeSnapshot () {
		return this->MOAIAbstractSnapshotSubjectVK_MakeSnapshot ();
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractSnapshotSubjectVK () {
		static_cast < MOAIAbstractSnapshotVK* >(( SNAPSHOT_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractSnapshotSubjectVK () {
	}
};

#endif
