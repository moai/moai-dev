// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUADDECK2D_H
#define	MOAIGFXQUADDECK2D_H

#include <moaicore/MOAIDeck2D.h>

class MOAITexture;

//================================================================//
// MOAIGfxQuadDeck2D
//================================================================//
/**	@name	MOAIGfxQuadDeck2D
	@text	Deck of textured quads.
*/
class MOAIGfxQuadDeck2D :
	public MOAIDeck2D {
private:

	USRef < MOAITexture >			mTexture;
	USLeanArray < USGLQuad >		mQuads;
	
	//----------------------------------------------------------------//
	static int	_reserve				( lua_State* L );
	static int	_setQuad				( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setTexture				( lua_State* L );
	static int	_setUVQuad				( lua_State* L );
	static int	_setUVRect				( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuadDeck2D )
	
	//----------------------------------------------------------------//
	bool		Bind					();
	void		DrawPatch				( u32 idx, float xOff, float yOff, float xScale, float yScale );
	USRect		GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
				MOAIGfxQuadDeck2D		();
				~MOAIGfxQuadDeck2D		();
	void		RegisterLuaClass		( USLuaState& state );
	void		RegisterLuaFuncs		( USLuaState& state );
	void		ScaleScreenCoords		( float xScale, float yScale );
	void		ScaleUVCoords			( float xScale, float yScale );
	STLString	ToString				();
};

#endif
