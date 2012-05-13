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

	// TODO: refactor; not all decks need this (or will be limited to this)
	MOAILuaSharedPtr < MOAIShader > mShader;
	MOAILuaSharedPtr < MOAIGfxState > mTexture;
	u32 mContentMask;

	MOAILuaSharedPtr < MOAIBoundsDeck > mBoundsDeck;

	SET ( u32, ContentMask, mContentMask )

	//----------------------------------------------------------------//
	static int				_setBoundsDeck			( lua_State* L );
	static int				_setShader				( lua_State* L );
	static int				_setTexture				( lua_State* L );

	//----------------------------------------------------------------//
	virtual void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	virtual USBox			GetBounds				( u32 idx ) = 0;

public:
	
	GET ( u32, ContentMask, mContentMask )
	
	//----------------------------------------------------------------//
	virtual bool			Contains				( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec );
	void					Draw					( u32 idx, MOAIDeckRemapper* remapper );
	void					Draw					( u32 idx, MOAIDeckRemapper* remapper, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	virtual USBox			GetBounds				() = 0;
	USBox					GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
	MOAIGfxState*			GetShader				();
	virtual MOAIGfxState*	GetShaderDefault		();
	MOAIGfxState*			GetTexture				();
							MOAIDeck				();
							~MOAIDeck				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
