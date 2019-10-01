// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTSUBJECTVK_H
#define MOAIABSTRACTSNAPSHOTSUBJECTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotCacheVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotSignatureVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

//================================================================//
// MOAIAbstractSnapshotSubjectVK
//================================================================//
template < typename SNAPSHOT_TYPE, typename SIGNATURE_TYPE >
class MOAIAbstractSnapshotSubjectVK {
public:

	typedef MOAIAbstractSnapshotCacheVK < SNAPSHOT_TYPE, SIGNATURE_TYPE > SnapshotCache;

protected:

	//----------------------------------------------------------------//
	virtual SnapshotCache*						MOAIAbstractSnapshotSubjectVK_GetCache				() { return NULL; }
	virtual const SIGNATURE_TYPE&				MOAIAbstractSnapshotSubjectVK_GetSignature			() const = 0;
	virtual SNAPSHOT_TYPE*						MOAIAbstractSnapshotSubjectVK_MakeSnapshot			() = 0;

public:
	
	//----------------------------------------------------------------//
	const SIGNATURE_TYPE& GetSignature () const {
		return this->MOAIAbstractSnapshotSubjectVK_GetSignature ();
	}
	
	//----------------------------------------------------------------//
	SnapshotCache* GetSnapshotCache () {
		return this->MOAIAbstractSnapshotSubjectVK_GetCache ();
	}
	
	//----------------------------------------------------------------//
	SNAPSHOT_TYPE* MakeSnapshot () {

		SnapshotCache* snapshotCache = this->GetSnapshotCache ();
		SNAPSHOT_TYPE* snapshot = snapshotCache ? snapshotCache->GetSnapshot ( this->GetSignature ()) : NULL;
		return snapshot ? snapshot : this->MOAIAbstractSnapshotSubjectVK_MakeSnapshot ();
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractSnapshotSubjectVK () {
		static_cast < MOAIAbstractSnapshotVK* >(( SNAPSHOT_TYPE* )NULL );
		static_cast < MOAIAbstractSnapshotSignatureVK < SIGNATURE_TYPE >* >(( SIGNATURE_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	~MOAIAbstractSnapshotSubjectVK () {
	}
};

#endif
