// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPRIM_H
#define	MOAIPRIM_H

#include <moaicore/MOAITransform2D.h>
#include <moaicore/MOAIPartition.h>

class MOAIContentLibrary2D;
class MOAIFrame;
class MOAIOverlapPrim2D;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIPrim
//================================================================//
/**	@brief Base class for primitives.
*/
class MOAIPrim :
	public MOAITransform2D,
	public USPartitionPrim < MOAIPrim > {
private:
	
	enum {
		FRAME_FROM_SOURCE,
		FRAME_FROM_PARENT,
		FRAME_FROM_DEFINE,
	};
	
	enum {
		SCALE_TO_FRAME,
		STRETCH_TO_FRAME,
	};
	
	u32				mFrameSource;
	u32				mFrameFit;
	
	USRect			mFrame;
	int				mPriority;
	
	//----------------------------------------------------------------//
	static int		_getPriority		( lua_State* L );
	static int		_setFrame			( lua_State* L );
	static int		_setFrameSource		( lua_State* L );
	static int		_setPriority		( lua_State* L );

public:
	
	enum {
		FRAME_EMPTY,
		FRAME_GLOBAL,
		FRAME_OK,
	};
	
	GET_SET ( int, Priority, mPriority )
	
	//----------------------------------------------------------------//
	virtual void					Draw					();
	virtual void					DrawDebug				();
	virtual void					GatherSurfaces			( MOAISurfaceSampler2D& sampler );
	virtual u32						GetLocalFrame			( USRect& frame );
	virtual MOAIOverlapPrim2D*		IsOverlapPrim2D			();
									MOAIPrim				();
									~MOAIPrim				();
	void							OnDepNodeUpdate			();
	void							RegisterLuaClass		( USLuaState& state );
	void							RegisterLuaFuncs		( USLuaState& state );
	STLString						ToString				();
};

#endif
