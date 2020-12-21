// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWDECK_H
#define	MOAIDRAWDECK_H

#include <moai-sim/MOAIStretchDeck.h>

//================================================================//
// MOAIDrawDeck
//================================================================//
/**	@lua MOAIDrawDeck
	@text Scriptable deck object for immediate mode drawing with MOAIDraw.
*/
class MOAIDrawDeck :
	public virtual MOAIDeck,
	public virtual MOAIStretchDeck {
private:

	ZLBounds				mBounds;

	MOAILuaMemberRef		mOnBounds;
	MOAILuaMemberRef		mOnDraw;

	//----------------------------------------------------------------//
	static int				_setBounds						( lua_State* L );
	static int				_setBoundsCallback				( lua_State* L );
	static int				_setDrawCallback				( lua_State* L );

	//----------------------------------------------------------------//
	void					SetBounds						( MOAILuaState& state, int idx );

	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	ZLBounds				MOAIDeck_GetBounds				();
	ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	void					MOAIDeck_Render					( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );

public:
	
	DECL_LUA_FACTORY ( MOAIDrawDeck )
	
	//----------------------------------------------------------------//
							MOAIDrawDeck					();
							~MOAIDrawDeck					();
};

#endif
