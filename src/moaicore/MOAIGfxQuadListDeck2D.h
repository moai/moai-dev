// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUADLISTDECK2D_H
#define	MOAIGFXQUADLISTDECK2D_H

#include <moaicore/MOAIDeck2D.h>

class MOAITexture;

//================================================================//
// USSpriteUVRect
//================================================================//
class USSpriteUVRect {
private:

	friend class MOAIGfxQuadListDeck2D;

	USRect	mRect;
	u32		mTransform;
};

//================================================================//
// USSpritePair
//================================================================//
class USSpritePair {
private:

	friend class MOAIGfxQuadListDeck2D;

	u32		mUVQuadID;
	u32		mQuadID;
};

//================================================================//
// USSprite
//================================================================//
class USSprite {
private:

	friend class MOAIGfxQuadListDeck2D;

	u32		mBasePair;
	u32		mTotalPairs;
};

//================================================================//
// MOAIGfxQuadListDeck2D
//================================================================//
/**	@name	MOAIGfxQuadListDeck2D
	@text	Deck of lists of textured quads. UV and model space quads are
			specified independently and associated via pairs. Pairs are referenced
			by lists sequentially. There may be multiple pairs with the same
			UV/model quad indicices if geometry is used in multiple lists.
*/
class MOAIGfxQuadListDeck2D :
	public MOAIDeck2D {
private:

	USRef < MOAITexture >			mTexture;
	
	USLeanArray < USQuad >			mUVQuads;
	USLeanArray < USQuad >			mQuads;
	USLeanArray < USSpritePair >	mPairs;
	USLeanArray < USSprite >		mSprites;
	
	//----------------------------------------------------------------//
	static int	_reserveLists			( lua_State* L );
	static int	_reservePairs			( lua_State* L );
	static int	_reserveQuads			( lua_State* L );
	static int	_reserveUVQuads			( lua_State* L );
	static int	_setList				( lua_State* L );
	static int	_setPair				( lua_State* L );
	static int	_setQuad				( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setTexture				( lua_State* L );
	static int	_setUVQuad				( lua_State* L );
	static int	_setUVRect				( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuadListDeck2D )
	
	//----------------------------------------------------------------//
	bool			Bind					();
	bool			Contains				( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec );
	void			DrawPatch				( u32 idx, float xOff, float yOff, float xScale, float yScale );
	USRect			GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
					MOAIGfxQuadListDeck2D	();
					~MOAIGfxQuadListDeck2D	();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			ReleaseTexture			();
	void			ReserveLists			( u32 total );
	void			ReservePairs			( u32 total );
	void			ReserveQuads			( u32 total );
	void			ReserveUVQuads			( u32 total );
	void			SetList					( u32 idx, u32 basePairID, u32 totalPairs );
	void			SetPair					( u32 idx, u32 uvRectID, u32 screenRectID );
	void			SetQuad					( u32 idx, USQuad& quad );
	void			SetRect					( u32 idx, USRect& rect );
	void			SetUVQuad				( u32 idx, USQuad& quad );
	void			SetUVRect				( u32 idx, USRect& rect );
	STLString		ToString				();
};

#endif
