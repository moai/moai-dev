// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUADDECK2D_H
#define	MOAIGFXQUADDECK2D_H

#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIQuadBrush.h>

class MOAITextureBase;

//================================================================//
// MOAIGfxQuadDeck2D
//================================================================//
/**	@lua	MOAIGfxQuadDeck2D
	@text	Deck of textured quads.
*/
class MOAIGfxQuadDeck2D :
	public MOAIDeck {
private:

	ZLLeanArray < MOAIQuadBrush >		mQuads;
	
	//----------------------------------------------------------------//
	static int	_reserve				( lua_State* L );
	static int	_setQuad				( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setUVQuad				( lua_State* L );
	static int	_setUVRect				( lua_State* L );
	static int	_transform				( lua_State* L );
	static int	_transformUV			( lua_State* L );
	
	//----------------------------------------------------------------//
	ZLBox		ComputeMaxBounds		();
	ZLBox		GetItemBounds			( u32 idx );
	
public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuadDeck2D )
	
	//----------------------------------------------------------------//
	void		DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	bool		Inside					( u32 idx, ZLVec3D vec, float pad );
				MOAIGfxQuadDeck2D		();
				~MOAIGfxQuadDeck2D		();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		Transform				( const ZLAffine3D& mtx );
	void		TransformUV				( const ZLAffine3D& mtx );
};

#endif
