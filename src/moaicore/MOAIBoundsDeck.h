// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOUNDSDECK_H
#define	MOAIBOUNDSDECK_H

#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIQuadBrush.h>

class MOAITextureBase;

//================================================================//
// MOAIBoundsDeck
//================================================================//
/**	@name	MOAIBoundsDeck
	@text	Deck of textured quads.
*/
class MOAIBoundsDeck :
	public MOAIDeck {
private:

	USLeanArray < USBox >	mBoundsArray;	// array of bounding boxes
	USLeanArray < u32 >		mIndexMap;		// maps bounding boxes onto deck item indices
	USBox					mMaxBounds;
	
	//----------------------------------------------------------------//
	static int	_computeMaxBounds		( lua_State* L );
	static int	_reserveBounds			( lua_State* L );
	static int	_reserveIndices			( lua_State* L );
	static int	_setBounds				( lua_State* L );
	static int	_setIndex				( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY ( MOAIBoundsDeck )
	
	//----------------------------------------------------------------//
	void		ComputeMaxBounds		();
	USBox		GetBounds				();
	USBox		GetBounds				( u32 idx );
				MOAIBoundsDeck			();
				~MOAIBoundsDeck			();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
