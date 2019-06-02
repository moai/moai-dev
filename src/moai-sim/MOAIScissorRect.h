// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISCISSORRECT_H
#define MOAISCISSORRECT_H

#include <moai-sim/MOAITransform.h>

//================================================================//
// MOAIScissorRect
//================================================================//
/**	@lua	MOAIScissorRect
	@text	Class for clipping props when drawing.
*/
class MOAIScissorRect :
	public MOAITransform {
private:
	
	ZLRect									mRect;
	MOAILuaSharedPtr < MOAIScissorRect >	mScissorRect;
	
	//----------------------------------------------------------------//
	static int		_getRect				( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setScissorRect			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIScissorRect )

	//----------------------------------------------------------------//
	ZLRect			GetScissorRect			( const ZLMatrix4x4& worldToWndMtx ) const;
					MOAIScissorRect			();
					~MOAIScissorRect		();
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
};

#endif
