// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTSUBJECTVK_H
#define MOAIABSTRACTSNAPSHOTSUBJECTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotCacheVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotSignatureVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

//================================================================//
// MOAIAbstractSnapshotFactoryVK
//================================================================//
template < typename SNAPSHOT_TYPE >
class MOAIAbstractSnapshotFactoryVK {
protected:

	//----------------------------------------------------------------//
	virtual SNAPSHOT_TYPE*		MOAIAbstractSnapshotFactoryVK_MakeSnapshot			() = 0;

public:

	//----------------------------------------------------------------//
	SNAPSHOT_TYPE* MakeSnapshot () {
		return this->MOAIAbstractSnapshotFactoryVK_MakeSnapshot ();
	}

	//----------------------------------------------------------------//
	MOAIAbstractSnapshotFactoryVK () {
		static_cast < MOAIAbstractSnapshotVK* >(( SNAPSHOT_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractSnapshotFactoryVK () {
	}
};

//================================================================//
// MOAIAbstractSnapshotSubjectVK
//================================================================//
template < typename SNAPSHOT_TYPE, typename SIGNATURE_TYPE = MOAIDummySnapshotSignatureVK >
class MOAIAbstractSnapshotSubjectVK :
	public MOAIAbstractSnapshotFactoryVK < SNAPSHOT_TYPE > {
public:

	typedef MOAIAbstractSnapshotCacheVK < SNAPSHOT_TYPE, SIGNATURE_TYPE > SnapshotCache;

protected:

	//----------------------------------------------------------------//
	SNAPSHOT_TYPE* MOAIAbstractSnapshotFactoryVK_MakeSnapshot () {
	
		const SIGNATURE_TYPE* signature = this->GetSignature ();
		SnapshotCache* cache = this->GetSnapshotCache ();

		SNAPSHOT_TYPE* snapshot = ( signature && cache ) ? cache->GetSnapshot ( signature ) : NULL;
		return snapshot ? snapshot : this->MOAIAbstractSnapshotSubjectVK_MakeSnapshot ();
	}

	//----------------------------------------------------------------//
	virtual SnapshotCache*						MOAIAbstractSnapshotSubjectVK_GetCache				() { return NULL; }
	virtual const SIGNATURE_TYPE*				MOAIAbstractSnapshotSubjectVK_GetSignature			() const { return NULL; }
	virtual SNAPSHOT_TYPE*						MOAIAbstractSnapshotSubjectVK_MakeSnapshot			() = 0;

public:
	
	//----------------------------------------------------------------//
	const SIGNATURE_TYPE* GetSignature () const {
		return this->MOAIAbstractSnapshotSubjectVK_GetSignature ();
	}
	
	//----------------------------------------------------------------//
	SnapshotCache* GetSnapshotCache () {
		return this->MOAIAbstractSnapshotSubjectVK_GetCache ();
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractSnapshotSubjectVK () {
		static_cast < MOAIAbstractSnapshotVK* >(( SNAPSHOT_TYPE* )NULL );
		static_cast < MOAIAbstractSnapshotSignatureVK < SIGNATURE_TYPE >* >(( SIGNATURE_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractSnapshotSubjectVK () {
	}
};

#endif
