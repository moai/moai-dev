// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUADDECK2D_H
#define	MOAIGFXQUADDECK2D_H

#include <moaicore/MOAIDeck.h>

class MOAITexture;

//================================================================//
// MOAIGfxQuadDeck2D
//================================================================//
/**	@brief Brush library class.
*/
class MOAIGfxQuadDeck2D :
	public MOAIDeck {
private:

	USRef < MOAITexture >			mTexture;
	USLeanArray < USGLQuad >		mQuads;
	
	//----------------------------------------------------------------//
	static int	_reserveBrushes			( lua_State* L );
	static int	_scaleCoords			( lua_State* L );
	static int	_scaleUVCoords			( lua_State* L );
	static int	_setQuad				( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setTexture				( lua_State* L );
	static int	_setUVQuad				( lua_State* L );
	static int	_setUVRect				( lua_State* L );
	
	//----------------------------------------------------------------//
	USGLQuad*	GetGLQuad				( u32 idx );
	
public:
	
	DECL_LUA_DATA ( MOAIGfxQuadDeck2D )
	
	//----------------------------------------------------------------//
	bool		Bind					();
	void		Draw					( const USAffine2D& transform, u32 idx );
	void		Draw					( const USAffine2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	USRect		GetBounds				( u32 idx );
				MOAIGfxQuadDeck2D		();
				~MOAIGfxQuadDeck2D		();
	void		RegisterLuaClass		( USLuaState& state );
	void		RegisterLuaFuncs		( USLuaState& state );
	void		ScaleScreenCoords		( float xScale, float yScale );
	void		ScaleUVCoords			( float xScale, float yScale );
	STLString	ToString				();
};

#endif
