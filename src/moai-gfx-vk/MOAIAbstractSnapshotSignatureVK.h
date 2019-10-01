// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTSIGNATUREVK_H
#define MOAIABSTRACTSNAPSHOTSIGNATUREVK_H

//================================================================//
// MOAIAbstractSnapshotSignatureVK
//================================================================//
template < typename SIGNATURE_TYPE >
class MOAIAbstractSnapshotSignatureVK {
protected:

	//----------------------------------------------------------------//
	virtual void			MOAIAbstractSnapshotSignatureVK_CloneFrom		( const SIGNATURE_TYPE& other ) = 0;
	virtual bool			MOAIAbstractSnapshotSignatureVK_IsSame			( const SIGNATURE_TYPE& other ) const = 0;

public:
	
	//----------------------------------------------------------------//
	void CloneFrom ( const SIGNATURE_TYPE& other ) {
		this->MOAIAbstractSnapshotSignatureVK_CloneFrom ( other );
	}
	
	//----------------------------------------------------------------//
	bool IsSame ( const SIGNATURE_TYPE& other ) const {
		return this->MOAIAbstractSnapshotSignatureVK_IsSame ( other );
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractSnapshotSignatureVK () {
		static_cast < MOAIAbstractSnapshotSignatureVK < SIGNATURE_TYPE >* >(( SIGNATURE_TYPE* )NULL );
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractSnapshotSignatureVK () {
	}
};

//================================================================//
// MOAIDummySnapshotSignatureVK
//================================================================//
class MOAIDummySnapshotSignatureVK :
	public MOAIAbstractSnapshotSignatureVK < MOAIDummySnapshotSignatureVK > {
protected:

	//----------------------------------------------------------------//
	void MOAIAbstractSnapshotSignatureVK_CloneFrom ( const MOAIDummySnapshotSignatureVK& other ) {
		UNUSED ( other );
	}
	
	//----------------------------------------------------------------//
	bool MOAIAbstractSnapshotSignatureVK_IsSame ( const MOAIDummySnapshotSignatureVK& other ) const {
		UNUSED ( other );
		return false;
	}

public:
	
	//----------------------------------------------------------------//
	MOAIDummySnapshotSignatureVK () {
	}
	
	//----------------------------------------------------------------//
	~MOAIDummySnapshotSignatureVK () {
	}
};

#endif
