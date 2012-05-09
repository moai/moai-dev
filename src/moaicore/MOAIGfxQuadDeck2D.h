// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUADDECK2D_H
#define	MOAIGFXQUADDECK2D_H

#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIQuadBrush.h>

class MOAITextureBase;

//================================================================//
// MOAIGfxQuadDeck2D
//================================================================//
/**	@name	MOAIGfxQuadDeck2D
	@text	Deck of textured quads.
*/
class MOAIGfxQuadDeck2D :
	public MOAIDeck2D {
private:

	USLeanArray < MOAIQuadBrush >		mQuads;
	
	//----------------------------------------------------------------//
	static int	_reserve				( lua_State* L );
	static int	_setQuad				( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setUVQuad				( lua_State* L );
	static int	_setUVRect				( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuadDeck2D )
	
	//----------------------------------------------------------------//
	void		DrawPatch				( u32 idx, float xOff, float yOff, float xScale, float yScale );
	USRect		GetRect					( u32 idx, MOAIDeckRemapper* remapper );
	USRect		GetRect					( );
				MOAIGfxQuadDeck2D		();
				~MOAIGfxQuadDeck2D		();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		ScaleScreenCoords		( float xScale, float yScale );
	void		ScaleUVCoords			( float xScale, float yScale );
};

#endif
