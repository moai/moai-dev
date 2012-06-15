// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISCISSORRECT_H
#define MOAISCISSORRECT_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// MOAIScissorRect
//================================================================//
/**	@name	MOAIScissorRect
	@text	Class for clipping props when drawing.
*/
class MOAIScissorRect :
	public MOAITransform {
private:
	
	USRect									mRect;
	MOAILuaSharedPtr < MOAIScissorRect >	mScissorRect;
	
	//----------------------------------------------------------------//
	static int		_getRect				( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setScissorRect			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIScissorRect )

	//----------------------------------------------------------------//
	USRect			GetScissorRect			( const USMatrix4x4& worldToWndMtx ) const;
					MOAIScissorRect			();
					~MOAIScissorRect		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
