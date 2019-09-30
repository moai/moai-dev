// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTSUBJECTVK_H
#define MOAIABSTRACTSNAPSHOTSUBJECTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotCacheVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotSignatureVK.h>
#include <moai-gfx-vk/MOAISnapshotVK.h>

//================================================================//
// MOAIAbstractSnapshotSubjectVK
//================================================================//
template < typename SNAPSHOT_TYPE, typename SIGNATURE_TYPE >
class MOAIAbstractSnapshotSubjectVK {
public:

	typedef MOAIAbstractSnapshotCacheVK < SNAPSHOT_TYPE, SIGNATURE_TYPE > SnapshotCache;

protected:

	ZLStrongPtr < SnapshotCache > mSnapshotCache;

	//----------------------------------------------------------------//
	virtual const SIGNATURE_TYPE&				MOAIAbstractSnapshotSubjectVK_GetSignature			() const = 0;
	virtual ZLStrongPtr < SNAPSHOT_TYPE >		MOAIAbstractSnapshotSubjectVK_MakeSnapshot			() const = 0;

public:
	
	//----------------------------------------------------------------//
	const SIGNATURE_TYPE& GetSignature () const {
		return this->MOAIAbstractSnapshotSubjectVK_GetSignature ();
	}
	
	//----------------------------------------------------------------//
	ZLStrongPtr < SNAPSHOT_TYPE > MakeSnapshot () {
	
		const SIGNATURE_TYPE& signature = this->GetSignature ();
		ZLStrongPtr < SNAPSHOT_TYPE > snapshot = this->mSnapshotCache ? this->mSnapshotCache->GetSnapshot ( signature ) : NULL;

		if ( !snapshot ) {
			snapshot = this->MOAIAbstractSnapshotSubjectVK_MakeSnapshot ();
			if ( this->mSnapshotCache ) {
				this->mSnapshotCache->StoreSnapshot ( signature, snapshot );
			}
		}
		return snapshot;
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractSnapshotSubjectVK () {
		static_cast < MOAISnapshotVK < SNAPSHOT_TYPE >* >(( SNAPSHOT_TYPE* )NULL );
		static_cast < MOAIAbstractSnapshotSignatureVK < SIGNATURE_TYPE >* >(( SIGNATURE_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	~MOAIAbstractSnapshotSubjectVK () {
	}
};

#endif
