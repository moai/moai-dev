// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK_H
#define	MOAIDECK_H

class MOAIDeckRemapper;
class MOAICellCoord;
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
	public virtual USLuaObject {
protected:

	enum {
		NO_CONTENT = 0xffffffff,
	};

	USRef < MOAIShader >	mShader;
	u32						mContentMask;

	SET ( u32, ContentMask, mContentMask )

	//----------------------------------------------------------------//
	static int			_setShader			( lua_State* L );

public:
	
	GET ( u32, ContentMask, mContentMask )
	
	//----------------------------------------------------------------//
	virtual bool		Bind						();
	virtual bool		Contains					( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec );
	virtual void		Draw						( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper );
	virtual void		Draw						( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 );
	virtual void		DrawDebug					( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper );
	virtual void		DrawDebug					( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 );
	virtual void		GatherSurfaces				( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler );
	virtual void		GatherSurfaces				( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1, MOAISurfaceSampler2D& sampler );
	virtual USRect		GetBounds					( u32 idx, MOAIDeckRemapper* remapper );
	virtual void		LoadShader					();
						MOAIDeck					();
						~MOAIDeck					();
	void				RegisterLuaClass			( USLuaState& state );
	void				RegisterLuaFuncs			( USLuaState& state );
};

#endif
