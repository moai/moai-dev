// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// Adapted from moai-assimp by Leandro Pezzente
// https://bitbucket.org/The-Mad-Pirate/moai-assimp

#ifndef MOAIASSIMPANIMATION_H
#define MOAIASSIMPANIMATION_H

#include <moai-assimp/MOAIAssimpSceneMember.h>
#include <moai-sim/MOAICamera.h>

//================================================================//
// MOAIAssimpAnimation
//================================================================//
// TODO: doxygen
class MOAIAssimpAnimation :
	public MOAIAssimpSceneMember {
private:

	aiAnimation*	mAssimpAnimation;
	
    //double mDuration;
    //double mTicksPerSecond;
    //unsigned int mNumChannels;
    //C_STRUCT aiNodeAnim** mChannels;
    //unsigned int mNumMeshChannels;
    //C_STRUCT aiMeshAnim** mMeshChannels;
	
	//----------------------------------------------------------------//
	static int		_countChannels				( lua_State* L );
	static int		_countPositionKeys			( lua_State* L );
	static int		_countRotationKeys			( lua_State* L );
	static int		_countScaleKeys				( lua_State* L );
	static int		_getAnimCurve				( lua_State* L );
	static int		_getChannelName				( lua_State* L );
	static int		_getDuration				( lua_State* L );
	static int		_getPositionKey				( lua_State* L );
	static int		_getRotationKey				( lua_State* L );
	static int		_getScaleKey				( lua_State* L );

	//----------------------------------------------------------------//
	aiNodeAnim*		GetChannel					( u32 channel );
	aiVectorKey*	GetPositionKey				( u32 channel, u32 key );
	aiQuatKey*		GetRotationKey				( u32 channel, u32 key );
	aiVectorKey*	GetScaleKey					( u32 channel, u32 key );

public:

	DECL_LUA_FACTORY ( MOAIAssimpAnimation )

	//----------------------------------------------------------------//
	MOAIAnimCurveBone*		GetAnimCurve				( u32 channel );
							MOAIAssimpAnimation			();
							~MOAIAssimpAnimation		();
	void					MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	void					SetAnimation				( aiAnimation* assimpAnimation );
};

#endif
