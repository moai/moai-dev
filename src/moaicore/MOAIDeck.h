// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK_H
#define	MOAIDECK_H

#include <moaicore/MOAILua.h>

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

	MOAILuaSharedPtr < MOAIShader > mShader;
	MOAILuaSharedPtr < MOAIGfxState > mTexture;
	u32 mContentMask;

	SET ( u32, ContentMask, mContentMask )

	//----------------------------------------------------------------//
	static int				_setShader				( lua_State* L );
	static int				_setTexture				( lua_State* L );

public:
	
	GET ( u32, ContentMask, mContentMask )
	
	//----------------------------------------------------------------//
	virtual bool			Contains				( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec );
	virtual void			Draw					( const USAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper );
	virtual void			Draw					( const USAffine3D& transform, bool reload, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, int cellAddr );
	virtual void			Draw					( const USAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 );
	virtual void			DrawDebug				( const USAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper );
	virtual void			DrawDebug				( const USAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 );
	virtual void			GatherSurfaces			( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler );
	virtual void			GatherSurfaces			( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1, MOAISurfaceSampler2D& sampler );
	virtual USBox			GetBounds				();
	virtual USBox			GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
	MOAIGfxState*			GetShader				();
	virtual MOAIGfxState*	GetShaderDefault		();
	MOAIGfxState*			GetTexture				();
							MOAIDeck				();
							~MOAIDeck				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
