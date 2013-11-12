//
//  MOAIMultiTextureDeck2D.h
//  libmoai
//
//  Created by Isaac Barrett on 11/7/13.
//
//

#ifndef	MOAIMULTITEXTUREDECK2D_H
#define	MOAIMULTITEXTUREDECK2D_H

#include <moaicore/MOAIGfxQuadDeck2D.h>
#include <moaicore/MOAILua.h>
//#include <moaicore/MOAIQuadBrush.h>

//================================================================//
// MOAIMultiTextureDeck2D
//================================================================//
/**	@name	MOAIMultiTextureDeck2D
 @text	Deck of textured quads using a different texture for each quad.
 */

class MOAIMultiTextureDeck2D : public MOAIGfxQuadDeck2D {
private:
	USLeanArray < MOAITextureBase* >	mTextures;
	USLeanArray < USColorVec >			mColors;
	
	//----------------------------------------------------------------//
	static int	_reserve				( lua_State* L );
	static int  _setColorAtIndex		( lua_State* L );
	static int	_setTexture				( lua_State* L );
	static int	_transformUVAtIndex		( lua_State* L );
	static int	_transformVertsAtIndex	( lua_State* L );
	
	
	//----------------------------------------------------------------//
public:
	
	DECL_LUA_FACTORY ( MOAIMultiTextureDeck2D )
	
	//----------------------------------------------------------------//
	void		DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	MOAIMultiTextureDeck2D		();
	~MOAIMultiTextureDeck2D		();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		SetColorAtIndex			( u32 idx, float r, float g, float b, float a);
	void		SetTexture				( u32 idx, MOAITextureBase* texture );
	void		TransformUVAtIndex		( u32 idx, const USAffine3D& mtx );
	void		TransformVertsAtIndex	( u32 idx, const USAffine3D& mtx );
	
	
};

#endif /* defined(MOAIMULTITEXTUREDECK2D_H) */
