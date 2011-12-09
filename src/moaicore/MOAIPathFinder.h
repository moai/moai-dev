// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATHFINDER_H
#define	MOAIPATHFINDER_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIPathState
//================================================================//
class MOAIPathState {
private:

	friend class MOAIPathFinder;

	int				mNodeID;
	MOAIPathState*	mParent;
	MOAIPathState*	mNext;
	
	float			mScore;
};

//================================================================//
// MOAIPathFinder
//================================================================//
class MOAIPathFinder :
	public virtual MOAILuaObject {
private:

	MOAIPathState*		mOpen;
	MOAIPathState*		mClosed;
	MOAIPathState*		mPath;
	
	int					mTargetNodeID;

	MOAIPathState*		mParentState; // used while expanding open set

	//----------------------------------------------------------------//
	bool		IsCandidate				( int nodeID );
	void		PushState				( int nodeID, float score );

public:
	
	//----------------------------------------------------------------//
				MOAIPathFinder			();
				~MOAIPathFinder			();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
