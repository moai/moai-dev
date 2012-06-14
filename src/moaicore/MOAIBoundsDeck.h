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
	@text	<p>Deck of bounding boxes. Bounding boxes are allocated in
			a separate array from that used for box indices. The index
			array is used to map deck indices onto bounding boxes. In
			other words there may be more indices then boxes thus
			allowing for re-use of boxes over multiple indices.</p>
			
			<p>The main purpose of the bounds deck is to override the
			default bounds of elements inside of another deck. The
			bounds deck may be attached to any other type of deck
			by using MOAIDeck's setBoundsDeck () method.</p>
*/
class MOAIBoundsDeck :
	public MOAIDeck {
private:

	friend class MOAIDeck;

	USLeanArray < USBox >	mBoundsArray;	// array of bounding boxes
	USLeanArray < u32 >		mIndexMap;		// maps bounding boxes onto deck item indices

	//----------------------------------------------------------------//
	static int	_reserveBounds			( lua_State* L );
	static int	_reserveIndices			( lua_State* L );
	static int	_setBounds				( lua_State* L );
	static int	_setIndex				( lua_State* L );
	
	//----------------------------------------------------------------//
	USBox		ComputeMaxBounds		();
	USBox		GetItemBounds			( u32 idx );
	
public:
	
	DECL_LUA_FACTORY ( MOAIBoundsDeck )
	
	//----------------------------------------------------------------//
				MOAIBoundsDeck			();
				~MOAIBoundsDeck			();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
