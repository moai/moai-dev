// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECKREMAPPER_H
#define	MOAIDECKREMAPPER_H

#include <moai-sim/MOAIDeckProxy.h>
#include <moai-sim/MOAINode.h>

//================================================================//
// MOAIDeckRemapper
//================================================================//
/**	@lua	MOAIDeckRemapper
	@text	Remap deck indices. Most useful for controlling animated tiles in tilemaps.
			All indices are exposed as attributes that may be connected by setAttrLink
			or driven using MOAIAnim or MOAIAnimCurveFloat.
*/
class MOAIDeckRemapper :
	public virtual MOAINode,
	public virtual MOAIDeckProxy {
protected:
	
	ZLSize						mBase;
	ZLLeanArray < ZLIndex >		mRemap;
	
	//----------------------------------------------------------------//
	static int			_reserve				( lua_State* L );
	static int			_setBase				( lua_State* L );
	static int			_setRemap				( lua_State* L );

	//----------------------------------------------------------------//
	ZLIndex				MOAIDeckProxy_Remap				( ZLIndex idx );
	bool				MOAINode_ApplyAttrOp			( u32 attrID, MOAIAttribute& attr, u32 op );

public:
	
	DECL_LUA_FACTORY ( MOAIDeckRemapper )
	
	//----------------------------------------------------------------//
						MOAIDeckRemapper				();
						~MOAIDeckRemapper				();
	void				RegisterLuaClass				( MOAILuaState& state );
	void				RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
