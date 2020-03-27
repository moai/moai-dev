// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGAPIOBJECTGL_H
#define	MOAIDRAWINGAPIOBJECTGL_H

#include <moai-gfx-gl/MOAIAbstractDrawingAPIGL.h>

//================================================================//
// MOAIAbstractDrawingAPIObjectGL
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingAPIObjectGL :
	public virtual MOAIAbstractDrawingAPIGL,
	public virtual MOAIAbstractDrawingAPIObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
 	static int			_loadShaderUniformGL		( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
						MOAIAbstractDrawingAPIObjectGL		();
	virtual				~MOAIAbstractDrawingAPIObjectGL		();
};

#endif
