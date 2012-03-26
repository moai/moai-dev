// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK2D_H
#define	MOAIDECK2D_H

#include <moaicore/MOAIDeck.h>

class MOAIGrid;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIDeck2D
//================================================================//
/**	@name MOAIDeck2D
	@text Base class for 2D decks.
*/
class MOAIDeck2D :
	public MOAIDeck {
public:
	
	//----------------------------------------------------------------//
	void				Draw				( const USAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper );
	void				Draw				( const USAffine3D& transform, bool reload, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, int cellAddr );
	void				Draw				( const USAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 );
	void				DrawDebug			( const USAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper );
	void				DrawDebug			( const USAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 );
	virtual void		DrawPatch			( u32 idx, float xOff, float yOff, float xScale, float yScale );
	void				GatherSurfaces		( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler );
	void				GatherSurfaces		( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1, MOAISurfaceSampler2D& sampler );
	USBox				GetBounds			();
	USBox				GetBounds			( u32 idx, MOAIDeckRemapper* remapper );
	virtual USRect		GetRect				( ) = 0;
	virtual USRect		GetRect				( u32 idx, MOAIDeckRemapper* remapper ) = 0;
						MOAIDeck2D			();
						~MOAIDeck2D			();
	void				RegisterLuaClass	( MOAILuaState& state );
	void				RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
