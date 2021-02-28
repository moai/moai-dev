// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATHTERRAINDECK_H
#define	MOAIPATHTERRAINDECK_H

#include <moai-sim/MOAIGridSpace.h>
#include <moai-sim/MOAIPathFinder.h>

//================================================================//
// MOAIPathTerrainDeck
//================================================================//
/**	@lua	MOAIPathTerrainDeck
	@text	Terrain specifications for use with pathfinding graphs. Contains
			indexed terrain types for graph nodes.
*/
class MOAIPathTerrainDeck :
	public MOAILuaObject {
private:

	friend class MOAIPathFinder;

	ZLLeanArray < u32 >		mMasks;
	ZLLeanArray < float >	mVectors;
	
	ZLSize					mDeckSize;
	ZLSize					mVectorSize;

	//----------------------------------------------------------------//
	static int		_getMask					( lua_State* L );
	static int		_getTerrainVec				( lua_State* L );
	static int		_setMask					( lua_State* L );
	static int		_setTerrainVec				( lua_State* L );
	static int		_reserve					( lua_State* L );

	//----------------------------------------------------------------//
	u32				GetMask						( u32 idx );
	float*			GetVector					( u32 idx );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIPathTerrainDeck )
	
	GET ( ZLSize, VectorSize, mVectorSize )
	
	//----------------------------------------------------------------//
					MOAIPathTerrainDeck			( ZLContext& context );
					~MOAIPathTerrainDeck		();
};

#endif
