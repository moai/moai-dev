// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCRIPTDECK_H
#define	MOAISCRIPTDECK_H

#include <moai-sim/MOAIDeck.h>

//================================================================//
// MOAIScriptDeck
//================================================================//
/**	@lua MOAIScriptDeck
	@text Scriptable deck object.
*/
class MOAIScriptDeck :
	public MOAIDeck {
private:

	ZLRect					mRect;

	MOAILuaMemberRef		mOnDraw;
	MOAILuaMemberRef		mOnRect;
	MOAILuaMemberRef		mOnTotalRect;

	//----------------------------------------------------------------//
	static int		_setDrawCallback		( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setRectCallback		( lua_State* L );
	static int		_setTotalRectCallback	( lua_State* L );

	//----------------------------------------------------------------//
	ZLBox			ComputeMaxBounds		();
	ZLBox			GetItemBounds			( u32 idx );

public:
	
	DECL_LUA_FACTORY ( MOAIScriptDeck )
	
	//----------------------------------------------------------------//
	void			DrawIndex				( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
					MOAIScriptDeck			();
					~MOAIScriptDeck			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
