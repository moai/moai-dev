// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISNAPSHOTVK_H
#define MOAISNAPSHOTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAICommandBufferVK.h>

//================================================================//
// MOAISnapshotFactoryVK
//================================================================//
template < typename SNAPSHOT_TYPE >
class MOAISnapshotFactoryVK :
	public virtual ZLRefCountedObject {
protected:

	//----------------------------------------------------------------//
	virtual SNAPSHOT_TYPE*		MOAISnapshotFactoryVK_GetSnapshot		() = 0;

public:
	
	//----------------------------------------------------------------//
	SNAPSHOT_TYPE* GetSnapshot () {
		SNAPSHOT_TYPE* snapshot = this->MOAISnapshotFactoryVK_GetSnapshot ();
		return snapshot;
	}
	
	//----------------------------------------------------------------//
	MOAISnapshotFactoryVK () {
		static_cast < MOAIAbstractSnapshotVK* >(( SNAPSHOT_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAISnapshotFactoryVK () {
	}
};

//================================================================//
// MOAISnapshotVK
//================================================================//
template < typename SNAPSHOT_TYPE >
class MOAISnapshotVK :
	public virtual MOAIAbstractSnapshotVK,
	public MOAISnapshotFactoryVK < SNAPSHOT_TYPE > {
public:

	typedef MOAISnapshotFactoryVK < SNAPSHOT_TYPE > Factory;
};

//================================================================//
// MOAISnapshotRefVK
//================================================================//
template < typename SNAPSHOT_TYPE >
class MOAISnapshotRefVK :
	public MOAISnapshotFactoryVK < SNAPSHOT_TYPE > {
protected:

	typedef MOAISnapshotFactoryVK < SNAPSHOT_TYPE > Factory;

	ZLStrongPtr < Factory > mObject;

	//----------------------------------------------------------------//
	SNAPSHOT_TYPE* MOAISnapshotFactoryVK_GetSnapshot () {
	
		return this->mObject ? this->mObject->GetSnapshot () : NULL;
	}

public:

	//----------------------------------------------------------------//
	inline operator bool () const {
		return this->mObject != 0;
	}

	//----------------------------------------------------------------//
	inline void operator = ( Factory* assign ) {
		this->mObject = assign;
	}

	//----------------------------------------------------------------//
	inline void operator = ( const MOAISnapshotRefVK < SNAPSHOT_TYPE >& assign ) {
	
		this->mObject = assign.mObject;
	}

	//----------------------------------------------------------------//
	MOAISnapshotRefVK () :
		mObject ( NULL ) {
	}

	//----------------------------------------------------------------//
	MOAISnapshotRefVK ( Factory* assign ) :
		mObject ( assign ) {
	}

	//----------------------------------------------------------------//
	MOAISnapshotRefVK ( const MOAISnapshotRefVK < SNAPSHOT_TYPE >& assign ) :
		mObject ( assign.mObject ) {
	}
	
	//----------------------------------------------------------------//
	~MOAISnapshotRefVK () {
	}
};

#endif
