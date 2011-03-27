// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSFORMBASE2D_H
#define	MOAITRANSFORMBASE2D_H

#include <moaicore/MOAINode.h>

//================================================================//
// MOAITransformBase2D
//================================================================//
class MOAITransformBase2D :
	public virtual MOAINode {
protected:
	
	USAffine2D		mLocalToWorldMtx;
	USAffine2D		mWorldToLocalMtx;

public:
	
	//----------------------------------------------------------------//
	const USAffine2D&	GetLocalToWorldMtx				();
	const USAffine2D&	GetWorldToLocalMtx				();
						MOAITransformBase2D				();
						~MOAITransformBase2D			();
	STLString			ToString						();
};

#endif
