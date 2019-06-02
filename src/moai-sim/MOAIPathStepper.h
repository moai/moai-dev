// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATHSTEPPER_H
#define	MOAIPATHSTEPPER_H

#include <moai-sim/MOAIPath.h>

class MOAIPath;

//================================================================//
// MOAIPathStepper
//================================================================//
// TODO: doxygen
class MOAIPathStepper :
	public virtual MOAILuaObject {
private:

	u32								mStepCount;
	u32								mTotalSteps;
	
	float							mStepSize;
	
	float							mEdgeBase;
	float							mEdgeLength;
	
	ZLVec2D							mEdge [ 3 ];
	ZLVec2D							mNormal;
	
	MOAILuaSharedPtr < MOAIPath>	mPath;
	
	ZLCubicBezierFlattener2D		mFlattener;
	ZLIndex							mSegmentIdx;
	ZLSize							mTotalSegments;
	float							mLength;

	//----------------------------------------------------------------//
	static int		_getLength				( lua_State* L );
	static int		_more					( lua_State* L );
	static int		_next					( lua_State* L );
	static int		_start					( lua_State* L );

	//----------------------------------------------------------------//
	bool			MoreVertices			();
	ZLVec2D			NextVertex				();

public:
	
	DECL_LUA_FACTORY ( MOAIPathStepper )
	
	GET ( float, Length, mLength )
	
	//----------------------------------------------------------------//
					MOAIPathStepper			();
					~MOAIPathStepper		();
	bool			More					();
	ZLVec2D			Next					();
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void			Start					( MOAIPath& path, float stepSize );
};

#endif
