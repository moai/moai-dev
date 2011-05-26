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
	void			Draw				( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper );
	virtual void	DrawPatch			( u32 idx, float xOff, float yOff, float xScale, float yScale );
	void			Draw				( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 );
	void			DrawDebug			( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper );
	void			DrawDebug			( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 );
	void			GatherSurfaces		( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler );
	void			GatherSurfaces		( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1, MOAISurfaceSampler2D& sampler );
					MOAIDeck2D			();
					~MOAIDeck2D			();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
};

#endif
