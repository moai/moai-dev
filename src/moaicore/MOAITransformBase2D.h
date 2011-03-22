// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSFORMBASE2D_H
#define	MOAITRANSFORMBASE2D_H

#include <moaicore/MOAINode.h>

//================================================================//
// MOAIDrawingMtx2D
//================================================================//
class MOAIDrawingMtx2D {
protected:
	
	USVec2D			mStretch;
	USAffine2D		mLocalToWorldMtx;

public:
	
	GET_SET ( USVec2D, Stretch, mStretch )
	
	//----------------------------------------------------------------//
	void				Append							( const USAffine2D& mtx );
	const USAffine2D&	GetLocalToWorldMtx				();
	USAffine2D			GetLocalToWorldMtxNoStretch		();
						MOAIDrawingMtx2D				();
						~MOAIDrawingMtx2D				();
	void				SetLocalToWorldMtx				( const USAffine2D& localToWorldMtx );
};

//================================================================//
// MOAITransformBase2D
//================================================================//
class MOAITransformBase2D :
	public virtual MOAINode,
	public MOAIDrawingMtx2D {
protected:
	
	USAffine2D		mWorldToLocalMtx;

public:
	
	//----------------------------------------------------------------//
	const USAffine2D&	GetWorldToLocalMtx				();
						MOAITransformBase2D				();
						~MOAITransformBase2D			();
	STLString			ToString						();
};

#endif
