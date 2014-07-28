// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK_H
#define	MOAIDECK_H

class MOAIBoundsDeck;
class MOAIDeckRemapper;
class MOAICellCoord;
class MOAICollisionShape;
class MOAIGfxState;
class MOAIGrid;
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
	MOAILuaSharedPtr < MOAIShader > mShader;
	MOAILuaSharedPtr < MOAIGfxState > mTexture;
	
	u32 mContentMask;
	SET ( u32, ContentMask, mContentMask )

	u32 mDefaultShaderID;

	ZLBox mMaxBounds;
	bool mBoundsDirty;

	//----------------------------------------------------------------//
	static int				_getTexture				( lua_State* L );
	static int				_setBoundsDeck			( lua_State* L );
	static int				_setShader				( lua_State* L );
	static int				_setTexture				( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLBox			ComputeMaxBounds		() = 0;
	virtual void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	virtual ZLBox			GetItemBounds			( u32 idx ) = 0;
	void					SetBoundsDirty			();

public:
	
	GET ( u32, ContentMask, mContentMask )
	
	//----------------------------------------------------------------//
	virtual bool			Contains				( u32 idx, MOAIDeckRemapper* remapper, const ZLVec2D& vec );
	void					Draw					( u32 idx, MOAIDeckRemapper* remapper );
	void					Draw					( u32 idx, MOAIDeckRemapper* remapper, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	ZLBox					GetBounds				();
	ZLBox					GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
	virtual void			GetCollisionShape		( MOAICollisionShape& shape );
	virtual void			GetGfxState				( MOAIDeckGfxState& gfxState );
							MOAIDeck				();
							~MOAIDeck				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
