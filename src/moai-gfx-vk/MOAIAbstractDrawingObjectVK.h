// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDRAWINGOBJECTVK_H
#define	MOAIABSTRACTDRAWINGOBJECTVK_H

#include <moai-gfx-vk/MOAIAbstractDrawingAPIVK.h>

//================================================================//
// MOAIAbstractDrawingObjectVK
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingObjectVK :
	public virtual MOAIAbstractDrawingAPIVK,
	public virtual MOAIAbstractDrawingObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
 	static int			_loadShaderUniformVK				( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
						MOAIAbstractDrawingObjectVK			();
	virtual				~MOAIAbstractDrawingObjectVK		();
};

#endif
