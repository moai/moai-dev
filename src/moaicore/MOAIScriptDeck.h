// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCRIPTDECK_H
#define	MOAISCRIPTDECK_H

#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIScriptDeck
//================================================================//
/**	@name MOAIScriptDeck
	@text Scriptable deck object.
*/
class MOAIScriptDeck :
	public MOAIDeck2D {
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

public:
	
	DECL_LUA_FACTORY ( MOAIScriptDeck )
	
	//----------------------------------------------------------------//
	void			DrawPatch				( u32 idx, float xOff, float yOff, float xScale, float yScale );
	USRect			GetRect					( u32 idx, MOAIDeckRemapper* remapper );
	USRect			GetRect					();
	MOAIGfxState*	GetShaderDefault		();
					MOAIScriptDeck			();
					~MOAIScriptDeck			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
