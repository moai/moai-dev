// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALHOLDER_H
#define	MOAIMATERIALHOLDER_H

#include <moai-gfx/MOAIMaterial.h>

//================================================================//
// MOAIMaterialHolder
//================================================================//
// TODO: doxygen
class MOAIMaterialHolder :
	public virtual MOAILuaObject {
protected:
	
	MOAIMaterial mMaterial;

	//----------------------------------------------------------------//
	static int			_getBlendMode			( lua_State* L );
	static int			_getCullMode			( lua_State* L );
	static int			_getDepthMask			( lua_State* L );
	static int			_getDepthTest			( lua_State* L );
	static int			_getLight				( lua_State* L );
	static int			_getShader				( lua_State* L );
	static int			_getTexture				( lua_State* L );
	static int			_setBlendMode			( lua_State* L );
	static int			_setCullMode			( lua_State* L );
	static int			_setDepthMask			( lua_State* L );
	static int			_setDepthTest			( lua_State* L );
	static int			_setLight				( lua_State* L );
	static int			_setShader				( lua_State* L );
	static int			_setTexture				( lua_State* L );

public:

	GET_CONST ( MOAIMaterial*, Material, &mMaterial )

	//----------------------------------------------------------------//
					MOAIMaterialHolder			();
					~MOAIMaterialHolder			();
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
