// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECKREMAPPER_H
#define	MOAIDECKREMAPPER_H

#include <moaicore/MOAINode.h>

class MOAIDeck;
class MOAILayer2D;

//================================================================//
// MOAIDeckRemapper
//================================================================//
class MOAIDeckRemapper :
	public virtual MOAINode {
protected:
	
	u32						mBase;
	USLeanArray < u32 >		mRemap;
	
	//----------------------------------------------------------------//
	static int		_reserve				( lua_State* L );
	static int		_setBase				( lua_State* L );
	static int		_setRemap				( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIDeckRemapper )
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
					MOAIDeckRemapper		();
					~MOAIDeckRemapper		();
	STLString		ToString				();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	u32				Remap					( u32 idx );
};

#endif
