// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISPRITEDECK2D_H
#define	MOAISPRITEDECK2D_H

#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIMaterialBatchHolder.h>
#include <moai-sim/MOAIQuadBrush.h>

//================================================================//
// MOAISprite
//================================================================//
class MOAISprite {
private:

	friend class MOAISpriteDeck2D;

	u32		mUVQuadID;
	u32		mQuadID;
	u32		mMaterialID;
};

//================================================================//
// MOAISpriteList
//================================================================//
class MOAISpriteList {
private:

	friend class MOAISpriteDeck2D;

	u32		mBaseSprite;
	u32		mTotalSprites;
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
	public MOAIMaterialBatchHolder {
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
	virtual ZLBounds				MOAIDeck_ComputeMaxBounds		();
	virtual void					MOAIDeck_Draw					( u32 idx );
	virtual ZLBounds				MOAIDeck_GetBounds				( u32 idx );
	virtual MOAICollisionShape*		MOAIDeck_GetCollisionShape		( u32 idx );
	virtual bool					MOAIDeck_Overlap				( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	virtual bool					MOAIDeck_Overlap				( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );

public:
	
	DECL_LUA_FACTORY ( MOAISpriteDeck2D )
	
	//----------------------------------------------------------------//
	static MOAIDeck*	AffirmDeck					( MOAILuaState& state, int idx );
	bool				Contains					( u32 idx, const ZLVec2D& vec );
	void				DrawIndex					( u32 idx, MOAIMaterialBatch* materials, ZLVec3D offset, ZLVec3D scale );
	bool				Inside						( u32 idx, MOAIMaterialBatch* materials, u32 granularity, ZLVec3D vec, float pad );
						MOAISpriteDeck2D			();
						~MOAISpriteDeck2D			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveLists				( u32 total );
	void				ReservePairs				( u32 total );
	void				ReserveQuads				( u32 total );
	void				ReserveUVQuads				( u32 total );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetList						( u32 idx, u32 basePairID, u32 totalPairs );
	void				SetPair						( u32 idx, u32 uvRectID, u32 screenRectID, u32 materialID );
	void				SetQuad						( u32 idx, ZLQuad& quad );
	void				SetRect						( u32 idx, ZLRect& rect );
	void				SetUVQuad					( u32 idx, ZLQuad& quad );
	void				SetUVRect					( u32 idx, ZLRect& rect );
	void				Transform					( const ZLAffine3D& mtx );
	void				TransformUV					( const ZLAffine3D& mtx );
};

#endif
