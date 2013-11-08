//
//  MOAIMultiTextureDeck2D.h
//  libmoai
//
//  Created by Isaac Barrett on 11/7/13.
//
//

#ifndef	MOAIMULTITEXTUREDECK2D_H
#define	MOAIMULTITEXTUREDECK2D_H

#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIQuadBrush.h>

//================================================================//
// MOAIMultiTextureDeck2D
//================================================================//
/**	@name	MOAIMultiTextureDeck2D
 @text	Deck of textured quads using a different texture for each quad.
 */

class MOAIMultiTextureDeck2D : public MOAIDeck {
private:
	USLeanArray < MOAIQuadBrush >		mQuads;
	USLeanArray < MOAITextureBase* >	mTextures;
	
	//----------------------------------------------------------------//
	static int	_reserve				( lua_State* L );
	static int	_setQuad				( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setTexture				( lua_State* L );
	static int	_setUVQuad				( lua_State* L );
	static int	_setUVRect				( lua_State* L );
	static int	_transform				( lua_State* L );
	static int	_transformUV			( lua_State* L );
	
	//----------------------------------------------------------------//
	USBox		ComputeMaxBounds		();
	USBox		GetItemBounds			( u32 idx );
public:
	
	DECL_LUA_FACTORY ( MOAIMultiTextureDeck2D )
	
	//----------------------------------------------------------------//
	void		DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	MOAIMultiTextureDeck2D		();
	~MOAIMultiTextureDeck2D		();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		Transform				( const USAffine3D& mtx );
	void		TransformUV				( const USAffine3D& mtx );
	
};

#endif /* defined(MOAIMULTITEXTUREDECK2D_H) */
