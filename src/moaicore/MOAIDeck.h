// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK_H
#define	MOAIDECK_H

#include <moaicore/MOAILua.h>

class MOAIBoundsDeck;
class MOAIDeckRemapper;
class MOAICellCoord;
class MOAIGfxState;
class MOAIGrid;
class MOAIShader;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIDeckGfxState
//================================================================//
class MOAIDeckGfxState {
private:

	MOAIGfxState*	mShader;
	MOAIGfxState*	mTexture;

public:

	GET ( MOAIGfxState*, Shader, mShader )
	GET ( MOAIGfxState*, Texture, mTexture )

	//----------------------------------------------------------------//
				MOAIDeckGfxState	();
	void		Reset				();
	void		SetShader			( MOAIGfxState* shader );
	void		SetTexture			( MOAIGfxState* texture );
};

//================================================================//
// MOAIDeck
//================================================================//
/**	@name	MOAIDeck
	@text	Base class for decks.
*/
class MOAIDeck :
	public virtual MOAILuaObject {
protected:

	enum {
		NO_CONTENT = 0xffffffff,
	};

	MOAILuaSharedPtr < MOAIBoundsDeck > mBoundsDeck; // bounds override

	// TODO: refactor; not all decks need thse (or will be limited to these)
	MOAILuaSharedPtr < MOAIGfxState > mShader;
	MOAILuaSharedPtr < MOAIGfxState > mTexture;
	
	u32 mContentMask;
	SET ( u32, ContentMask, mContentMask )

	u32 mDefaultShaderID;

	USBox mMaxBounds;
	bool mBoundsDirty;

	//----------------------------------------------------------------//
	static int				_setBoundsDeck			( lua_State* L );
	static int				_setShader				( lua_State* L );
	static int				_setTexture				( lua_State* L );

	//----------------------------------------------------------------//
	virtual USBox			ComputeMaxBounds		() = 0;
	virtual void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	virtual USBox			GetItemBounds			( u32 idx ) = 0;
	void					SetBoundsDirty			();

public:
	
	GET ( u32, ContentMask, mContentMask )
	
	//----------------------------------------------------------------//
	virtual bool			Contains				( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec );
	void					Draw					( u32 idx, MOAIDeckRemapper* remapper );
	void					Draw					( u32 idx, MOAIDeckRemapper* remapper, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	USBox					GetBounds				();
	USBox					GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
	virtual void			GetGfxState				( MOAIDeckGfxState& gfxState );
							MOAIDeck				();
							~MOAIDeck				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
