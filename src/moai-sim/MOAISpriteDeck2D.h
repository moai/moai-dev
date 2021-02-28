// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISPRITEDECK2D_H
#define	MOAISPRITEDECK2D_H

#include <moai-sim/MOAIHasHitMaskBatch.h>

//================================================================//
// MOAISprite
//================================================================//
class MOAISprite {
public:

	friend class MOAISpriteDeck2D;

	ZLIndex		mUVQuadID;
	ZLIndex		mQuadID;
	ZLIndex		mMaterialID;
};

//================================================================//
// MOAISpriteBrush
//================================================================//
class MOAISpriteBrush :
	public MOAIQuadBrush {
public:

	ZLIndex		mMaterialID;
};

//================================================================//
// MOAISpriteList
//================================================================//
class MOAISpriteList {
public:

	friend class MOAISpriteDeck2D;

	ZLIndex		mBaseSprite;
	ZLSize		mTotalSprites;
};

//================================================================//
// MOAISpriteDeck2DCallable
//================================================================//
class MOAISpriteDeck2DCallable :
	public MOAIAbstractGfxScriptCallback {
public:

	friend class MOAISpriteDeck2D;

	MOAIGfxMgr&			mGfxMgr;
	MOAISpriteBrush		mBrush;

	//----------------------------------------------------------------//
	void 		MOAIAbstractGfxScriptCallback_Call 		();
				MOAISpriteDeck2DCallable				( MOAIGfxMgr& gfxMgr );
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
	public virtual MOAIDeck,
	public virtual MOAIHasGfxScriptBatchesForPhases,
	public virtual MOAIHasHitMaskBatch {
private:
	
	ZLLeanArray < ZLQuad >				mUVQuads;
	ZLLeanArray < ZLQuad >				mQuads;
	ZLLeanArray < MOAISprite >			mSprites;
	ZLLeanArray < MOAISpriteList >		mSpriteLists;
	
	//----------------------------------------------------------------//
	static int				_getQuad						( lua_State* L );
	static int				_getRect						( lua_State* L );
	static int				_getUVQuad						( lua_State* L );
	static int				_getUVRect						( lua_State* L );
	static int				_reserveQuads					( lua_State* L );
	static int				_reserveSprites					( lua_State* L );
	static int				_reserveSpriteLists				( lua_State* L );
	static int				_reserveUVQuads					( lua_State* L );
	static int				_setQuad						( lua_State* L );
	static int				_setRect						( lua_State* L );
	static int				_setSprite						( lua_State* L );
	static int				_setSpriteList					( lua_State* L );
	static int				_setUVQuad						( lua_State* L );
	static int				_setUVRect						( lua_State* L );
	static int				_transform						( lua_State* L );
	static int				_transformUV					( lua_State* L );

	//----------------------------------------------------------------//
	ZLSize					CountSpriteLists				() const;
	ZLQuad					GetQuad							( ZLIndex index ) const;
	MOAISprite				GetSprite						( ZLIndex index ) const;
	MOAISpriteBrush			GetSpriteBrush					( ZLIndex index ) const;
	MOAISpriteList			GetSpriteList					( ZLIndex index ) const;
	ZLQuad					GetUVQuad						( ZLIndex index ) const;

	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	ZLBounds				MOAIDeck_GetBounds				();
	ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	MOAIPickResult			MOAIDeck_PickByPoint			( ZLIndex idx, ZLVec3D loc );
	MOAIPickResult			MOAIDeck_PickByRay				( ZLIndex idx, ZLVec3D loc, ZLVec3D normal );
	void					MOAIDeck_Render					( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );

public:
	
	DECL_LUA_FACTORY ( MOAISpriteDeck2D )
	
	//----------------------------------------------------------------//
	static MOAIDeck*		AffirmDeck						( MOAILuaState& state, int idx );
	bool					Contains						( ZLIndex idx, const ZLVec2D& vec );
	void					DrawIndex						( ZLIndex idx, MOAIGfxScriptBatch* materials, ZLVec3D offset, ZLVec3D scale );
	bool					Inside							( ZLIndex idx, MOAIGfxScriptBatch* materials, u32 granularity, ZLVec3D vec, float pad );
							MOAISpriteDeck2D				( ZLContext& context );
							~MOAISpriteDeck2D				();
	void					ReserveLists					( ZLSize total );
	void					ReservePairs					( ZLSize total );
	void					ReserveQuads					( ZLSize total );
	void					ReserveUVQuads					( ZLSize total );
	void					SetList							( ZLIndex idx, ZLIndex basePairID, ZLSize totalPairs );
	void					SetPair							( ZLIndex idx, ZLIndex uvRectID, ZLIndex screenRectID, ZLIndex materialID );
	void					SetQuad							( ZLIndex idx, ZLQuad& quad );
	void					SetRect							( ZLIndex idx, ZLRect& rect );
	void					SetUVQuad						( ZLIndex idx, ZLQuad& quad );
	void					SetUVRect						( ZLIndex idx, ZLRect& rect );
	void					Transform						( const ZLAffine3D& mtx );
	void					TransformUV						( const ZLAffine3D& mtx );
};

#endif
