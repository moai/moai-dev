// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISPINESKELETONDATA_H
#define MOAISPINESKELETONDATA_H

#include <spine/spine.h>

//================================================================//
// MOAISpineSkeletonData
//================================================================//
/**	@name	MOAISpineSkeletonData
	@text	Class for reading spine json and atlas files.
			Keeps skeleton data that can be shared among many
			instances for faster loading and memory efficiency.
*/
class MOAISpineSkeletonData :
	public virtual MOAILuaObject {
private:
	
	friend class MOAISpineSkeleton;
		
	//----------------------------------------------------------------//
	static int		_load				( lua_State* L );

protected:
	spSkeletonData* mSkeletonData;
	spAtlas*		mAtlas;

public:
	
	DECL_LUA_FACTORY ( MOAISpineSkeletonData )
	
	//----------------------------------------------------------------//
					MOAISpineSkeletonData		();
					~MOAISpineSkeletonData		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
