// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCRIPTDECK_H
#define	MOAISCRIPTDECK_H

#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIScriptDeck
//================================================================//
/**	@name MOAIScriptDeck
	@text Scriptable deck object.
*/
class MOAIScriptDeck :
	public MOAIDeck {
private:

	USRect				mRect;

	MOAILuaLocal		mOnDraw;
	MOAILuaLocal		mOnRect;
	MOAILuaLocal		mOnTotalRect;

	//----------------------------------------------------------------//
	static int		_setDrawCallback		( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setRectCallback		( lua_State* L );
	static int		_setTotalRectCallback	( lua_State* L );

	//----------------------------------------------------------------//
	USBox			ComputeMaxBounds		();
	USBox			GetItemBounds			( u32 idx );

public:
	
	DECL_LUA_FACTORY ( MOAIScriptDeck )
	
	//----------------------------------------------------------------//
	void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
					MOAIScriptDeck			();
					~MOAIScriptDeck			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
