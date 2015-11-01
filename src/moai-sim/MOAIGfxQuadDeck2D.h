// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUADDECK2D_H
#define	MOAIGFXQUADDECK2D_H

#include <moai-sim/MOAIStandardDeck.h>
#include <moai-sim/MOAIQuadBrush.h>

class MOAISingleTexture;

//================================================================//
// MOAIGfxQuadDeck2D
//================================================================//
/**	@lua	MOAIGfxQuadDeck2D
	@text	Deck of textured quads.
*/
class MOAIGfxQuadDeck2D :
	public MOAIStandardDeck {
private:

	ZLLeanArray < MOAIQuadBrush >		mQuads;
	ZLLeanArray < u32 >					mMaterialIDs;
	
	//----------------------------------------------------------------//
	static int	_getQuad				( lua_State* L );
	static int	_getRect				( lua_State* L );
	static int	_getUVQuad				( lua_State* L );
	static int	_getUVRect				( lua_State* L );
	static int	_reserve				( lua_State* L );
	static int	_setMaterialID			( lua_State* L );
	static int	_setQuad				( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setUVQuad				( lua_State* L );
	static int	_setUVRect				( lua_State* L );
	static int	_transform				( lua_State* L );
	static int	_transformUV			( lua_State* L );
	
	//----------------------------------------------------------------//
	MOAIQuadBrush&		AffirmQuad				( u32 idx );
	ZLBox				ComputeMaxBounds		();
	ZLBox				GetItemBounds			( u32 idx );
	
public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuadDeck2D )
	
	//----------------------------------------------------------------//
	void		DrawIndex				( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	bool		Inside					( u32 idx, MOAIMaterialBatch& materials, u32 granularity, ZLVec3D vec, float pad );
				MOAIGfxQuadDeck2D		();
				~MOAIGfxQuadDeck2D		();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		Transform				( const ZLAffine3D& mtx );
	void		TransformUV				( const ZLAffine3D& mtx );
};

#endif
