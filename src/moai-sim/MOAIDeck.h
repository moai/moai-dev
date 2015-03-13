// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK_H
#define	MOAIDECK_H

class MOAIBoundsDeck;
class MOAICellCoord;
class MOAICollisionShape;
class MOAIColor;
class MOAIGfxState;
class MOAIGrid;
class MOAIImage;
class MOAIQuadBrush;
class MOAIShader;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIDeckGfxState
//================================================================//
class MOAIDeckGfxState {
private:

	MOAIShader*		mShader;
	MOAIGfxState*	mTexture;

public:

	GET ( MOAIShader*, Shader, mShader )
	GET ( MOAIGfxState*, Texture, mTexture )

	//----------------------------------------------------------------//
				MOAIDeckGfxState	();
	void		Reset				();
	void		SetShader			( MOAIShader* shader );
	void		SetTexture			( MOAIGfxState* texture );
};

//================================================================//
// MOAIDeck
//================================================================//
/**	@lua	MOAIDeck
	@text	Base class for decks.
*/
class MOAIDeck :
	public virtual MOAILuaObject {
protected:

	enum {
		NO_CONTENT = 0xffffffff,
	};

	MOAILuaSharedPtr < MOAIBoundsDeck > mBoundsDeck; // bounds override

	// TODO: refactor; not all decks need these (or will be limited to these)
	// should probebly go into a 'graphics deck' and play off of the developing
	// notion of 'aspects'
	MOAILuaSharedPtr < MOAIShader >		mShader;
	MOAILuaSharedPtr < MOAIGfxState >	mTexture;
	MOAILuaSharedPtr < MOAIImage >		mHitMask;
	
	u32		mHitColorScalar;
	u32		mHitColorThreshold;
	
	//u32 mContentMask;
	//SET ( u32, ContentMask, mContentMask )

	u32		mDefaultShaderID;

	ZLBox	mMaxBounds;
	bool	mBoundsDirty;
	
	u32		mHitGranularity;

	//----------------------------------------------------------------//
	static int				_getTexture				( lua_State* L );
	static int				_setBoundsDeck			( lua_State* L );
	static int				_setHitGranularity		( lua_State* L );
	static int				_setHitMask				( lua_State* L );
	static int				_setHitMaskScalar		( lua_State* L );
	static int				_setHitMaskThreshold	( lua_State* L );
	static int				_setShader				( lua_State* L );
	static int				_setTexture				( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLBox			ComputeMaxBounds		() = 0;
	virtual void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	virtual ZLBox			GetItemBounds			( u32 idx ) = 0;
	void					SetBoundsDirty			();
	bool					TestHit					( float x, float y ); // in local (uv) space of the mask
	bool					TestHit					( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad

public:
	
	enum {
		HIT_TEST_COARSE,	// no hit test will be performed; only the prop's bounds will be used
		HIT_TEST_MEDIUM,	// implementation dependent
		HIT_TEST_FINE,		// implementation dependent
	};
	
	//GET ( u32, ContentMask, mContentMask )
	GET ( u32, HitGranularity, mHitGranularity )
	
	//----------------------------------------------------------------//
	virtual bool			Contains				( u32 idx, const ZLVec2D& vec );
	void					Draw					( u32 idx );
	void					Draw					( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	ZLBox					GetBounds				();
	ZLBox					GetBounds				( u32 idx );
	virtual void			GetCollisionShape		( MOAICollisionShape& shape );
	virtual void			GetGfxState				( MOAIDeckGfxState& gfxState );
	virtual bool			Inside					( u32 idx, ZLVec3D vec, float pad );
							MOAIDeck				();
							~MOAIDeck				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
