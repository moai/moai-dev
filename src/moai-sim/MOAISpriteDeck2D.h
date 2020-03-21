// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISPRITEDECK2D_H
#define	MOAISPRITEDECK2D_H

#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIQuadBrush.h>

//================================================================//
// MOAISprite
//================================================================//
class MOAISprite {
private:

	friend class MOAISpriteDeck2D;

	ZLIndex		mUVQuadID;
	ZLIndex		mQuadID;
	ZLIndex		mMaterialID;
};

//================================================================//
// MOAISpriteList
//================================================================//
class MOAISpriteList {
private:

	friend class MOAISpriteDeck2D;

	ZLIndex		mBaseSprite;
	ZLSize		mTotalSprites;
};

//================================================================//
// MOAISpriteDeck2DCallable
//================================================================//
class MOAISpriteDeck2DCallable :
	public MOAIAbstractGfxComposerCallable {
private:

	friend class MOAISpriteDeck2D;

	MOAIQuadBrush	mBrush;

	//----------------------------------------------------------------//
	void 		MOAIAbstractGfxComposerCallable_Call 		();
};

//================================================================//
// MOAISpriteDeck2D
//================================================================//
/**	@lua	MOAISpriteDeck2D
	@text	Deck of lists of textured quads. UV and model space quads are
			specified independently and associated via pairs. Pairs are referenced
			by lists sequentially. There may be multiple pairs with the same
			UV/model quad indices if geometry is used in multiple lists.
*/
class MOAISpriteDeck2D :
	public MOAIDeck,
	public MOAIHasGfxComposer {
private:
	
	ZLLeanArray < ZLQuad >				mUVQuads;
	ZLLeanArray < ZLQuad >				mQuads;
	ZLLeanArray < MOAISprite >			mSprites;
	ZLLeanArray < MOAISpriteList >		mSpriteLists;
	
	//----------------------------------------------------------------//
	static int	_getQuad				( lua_State* L );
	static int	_getRect				( lua_State* L );
	static int	_getUVQuad				( lua_State* L );
	static int	_getUVRect				( lua_State* L );
	static int	_reserveQuads			( lua_State* L );
	static int	_reserveSprites			( lua_State* L );
	static int	_reserveSpriteLists		( lua_State* L );
	static int	_reserveUVQuads			( lua_State* L );
	static int	_setQuad				( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setSprite				( lua_State* L );
	static int	_setSpriteList			( lua_State* L );
	static int	_setUVQuad				( lua_State* L );
	static int	_setUVRect				( lua_State* L );
	static int	_transform				( lua_State* L );
	static int	_transformUV			( lua_State* L );

	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_SerializeIn					( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void					_SerializeOut					( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	ZLBounds				MOAIDeck_ComputeMaxAABB			();
	void					MOAIDeck_Draw					( ZLIndex idx );
	ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( ZLIndex idx );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );

public:
	
	DECL_LUA_FACTORY ( MOAISpriteDeck2D )
	
	//----------------------------------------------------------------//
	static MOAIDeck*	AffirmDeck					( MOAILuaState& state, int idx );
	bool				Contains					( ZLIndex idx, const ZLVec2D& vec );
	void				DrawIndex					( ZLIndex idx, MOAIGfxComposerBatch* materials, ZLVec3D offset, ZLVec3D scale );
	bool				Inside						( ZLIndex idx, MOAIGfxComposerBatch* materials, u32 granularity, ZLVec3D vec, float pad );
						MOAISpriteDeck2D			();
						~MOAISpriteDeck2D			();
	void				ReserveLists				( ZLSize total );
	void				ReservePairs				( ZLSize total );
	void				ReserveQuads				( ZLSize total );
	void				ReserveUVQuads				( ZLSize total );
	void				SetList						( ZLIndex idx, ZLIndex basePairID, ZLSize totalPairs );
	void				SetPair						( ZLIndex idx, ZLIndex uvRectID, ZLIndex screenRectID, ZLIndex materialID );
	void				SetQuad						( ZLIndex idx, ZLQuad& quad );
	void				SetRect						( ZLIndex idx, ZLRect& rect );
	void				SetUVQuad					( ZLIndex idx, ZLQuad& quad );
	void				SetUVRect					( ZLIndex idx, ZLRect& rect );
	void				Transform					( const ZLAffine3D& mtx );
	void				TransformUV					( const ZLAffine3D& mtx );
};

#endif
