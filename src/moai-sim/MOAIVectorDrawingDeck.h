// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORDRAWINGDECK_H
#define	MOAIVECTORDRAWINGDECK_H

#include <moai-sim/MOAIDeck.h>

class MOAIVectorDrawing;

//================================================================//
// MOAIVectorDrawingDeck
//================================================================//
/**	@name	MOAIVectorDrawingDeck
	@text	Deck of vector drawings.
*/
class MOAIVectorDrawingDeck :
	public MOAIDeck {
private:

	ZLLeanArray < MOAILuaSharedPtr < MOAIVectorDrawing > > mDrawings;

	//----------------------------------------------------------------//
	static int		_reserve					( lua_State* L );
	static int		_setDrawing					( lua_State* L );

	//----------------------------------------------------------------//
	ZLBox			ComputeMaxBounds			();
	ZLBox			GetItemBounds				( u32 idx );

public:
	
	DECL_LUA_FACTORY ( MOAIVectorDrawingDeck )
	
	//----------------------------------------------------------------//
	void			Clear						();
	void			DrawIndex					( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
					MOAIVectorDrawingDeck		();
					~MOAIVectorDrawingDeck		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
