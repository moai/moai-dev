// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSFORMBASE_H
#define	MOAITRANSFORMBASE_H

#include <moaicore/MOAINode.h>

//================================================================//
// MOAITransformBase
//================================================================//
class MOAITransformBase :
	public virtual MOAINode {
protected:
	
	USAffine2D		mLocalToWorldMtx;
	USAffine2D		mWorldToLocalMtx;

public:
	
	//----------------------------------------------------------------//
	const USAffine2D&	GetLocalToWorldMtx				();
	const USAffine2D&	GetWorldToLocalMtx				();
						MOAITransformBase				();
						~MOAITransformBase			();
	STLString			ToString						();
};

#endif
