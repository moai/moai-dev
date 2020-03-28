// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGAPIOBJECTVK_H
#define	MOAIDRAWINGAPIOBJECTVK_H

#include <moai-gfx-vk/MOAIAbstractDrawingAPIVK.h>

//================================================================//
// MOAIAbstractDrawingAPIObjectVK
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingAPIObjectVK :
	public virtual MOAIAbstractDrawingAPIVK,
	public virtual MOAIAbstractDrawingAPIObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
 	static int			_loadShaderUniformVK		( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
						MOAIAbstractDrawingAPIObjectVK		();
	virtual				~MOAIAbstractDrawingAPIObjectVK		();
};

#endif
