// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATHTERRAINDECK_H
#define	MOAIPATHTERRAINDECK_H

#include <moaicore/MOAIGridSpace.h>
#include <moaicore/MOAIPathFinder.h>

//================================================================//
// MOAIPathTerrainDeck
//================================================================//
/**	@name	MOAIPathTerrainDeck
	@text	Terrain specifications for use with pathfinding graphs. Contains
			indexed terrain types for graph nodes.
*/
class MOAIPathTerrainDeck :
	public MOAILuaObject {
private:

	friend class MOAIPathFinder;

	USLeanArray < u32 >		mMasks;
	USLeanArray < float >	mVectors;
	
	u32						mDeckSize;
	u32						mVectorSize;

	//----------------------------------------------------------------//
	static int		_getMask					( lua_State* L );
	static int		_getTerrainVec				( lua_State* L );
	static int		_setMask					( lua_State* L );
	static int		_setTerrainVec				( lua_State* L );
	static int		_reserve					( lua_State* L );

	//----------------------------------------------------------------//
	u32				GetMask						( u32 idx );
	float*			GetVector					( u32 idx );

public:
	
	DECL_LUA_FACTORY ( MOAIPathTerrainDeck )
	
	GET ( u32, VectorSize, mVectorSize )
	
	//----------------------------------------------------------------//
					MOAIPathTerrainDeck			();
					~MOAIPathTerrainDeck		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
