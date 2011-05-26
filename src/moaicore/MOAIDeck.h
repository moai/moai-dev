// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK_H
#define	MOAIDECK_H

class MOAIDeckRemapper;
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
private:

	USRef < MOAIShader >	mDefaultShader;
	u32						mContentMask;

	//----------------------------------------------------------------//
	static int			_setDefaultShader			( lua_State* L );

protected:

	enum {
		NO_CONTENT = 0xffffffff,
	};

	SET ( u32, ContentMask, mContentMask )

public:
	
	GET ( u32, ContentMask, mContentMask )
	
	//----------------------------------------------------------------//
	virtual bool		Bind						();
	virtual bool		Contains					( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec );
	virtual void		Draw						( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper );
	virtual void		Draw						( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 );
	virtual void		DrawDebug					( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper );
	virtual void		DrawDebug					( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 );
	virtual void		GatherSurfaces				( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler );
	virtual void		GatherSurfaces				( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1, MOAISurfaceSampler2D& sampler );
	virtual USRect		GetBounds					( u32 idx, MOAIDeckRemapper* remapper );
						MOAIDeck					();
						~MOAIDeck					();
	void				RegisterLuaClass			( USLuaState& state );
	void				RegisterLuaFuncs			( USLuaState& state );
};

#endif
