// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWDECK_H
#define	MOAIDRAWDECK_H

#include <moai-sim/MOAIStretchDeck.h>

//================================================================//
// MOAIDrawDeck
//================================================================//
/**	@lua MOAIDrawDeck
	@text Scriptable deck objectf for immediate mode drawing with MOAIDraw.
*/
class MOAIDrawDeck :
	public MOAIStretchDeck {
private:

	ZLBounds				mBounds;

	MOAILuaMemberRef		mOnBounds;
	MOAILuaMemberRef		mOnDraw;

	//----------------------------------------------------------------//
	static int				_setBounds						( lua_State* L );
	static int				_setBoundsCallback				( lua_State* L );
	static int				_setDrawCallback				( lua_State* L );

	//----------------------------------------------------------------//
	void					SetBounds						( MOAILuaState& state, int idx );

	//----------------------------------------------------------------//
	ZLBounds				MOAIDeck_ComputeMaxBounds		();
	void					MOAIDeck_Draw					( ZLIndex idx );
	ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( ZLIndex idx );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );

public:
	
	DECL_LUA_FACTORY ( MOAIDrawDeck )
	
	//----------------------------------------------------------------//
							MOAIDrawDeck					();
							~MOAIDrawDeck					();
	void					RegisterLuaClass				( MOAILuaState& state );
	void					RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
