// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWABLE_H
#define	MOAIDRAWABLE_H

#include <moai-sim/MOAIPartitionHull.h>

//================================================================//
// MOAIDrawable
//================================================================//
class MOAIDrawable :
	public virtual RTTIBase {
private:

	//----------------------------------------------------------------//
	virtual void		MOAIDrawable_Draw				( int subPrimID ) = 0;
	virtual void		MOAIDrawable_DrawDebug			( int subPrimID ) {};

public:

	//----------------------------------------------------------------//
	static void		Draw				( MOAILuaMemberRef& ref );
	static void		Draw				( MOAILuaState& state, int idx );
	void			Draw				( int subPrimID = MOAIPartitionHull::NO_SUBPRIM_ID ) { MOAIDrawable_Draw ( subPrimID ); }
	void			DrawDebug			( int subPrimID = MOAIPartitionHull::NO_SUBPRIM_ID ) { MOAIDrawable_DrawDebug ( subPrimID ); }
};

#endif
