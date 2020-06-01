// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDRAWINGLUAAPIVK_H
#define	MOAIABSTRACTDRAWINGLUAAPIVK_H

#include <moai-gfx-vk/MOAIAbstractDrawingAPIVK.h>

//================================================================//
// MOAIAbstractDrawingLuaAPIVK
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingLuaAPIVK :
	public virtual MOAIAbstractDrawingAPIVK,
	public virtual MOAIAbstractDrawingLuaAPI {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
 	static int			_loadShaderUniformVK				( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
						MOAIAbstractDrawingLuaAPIVK			();
	virtual				~MOAIAbstractDrawingLuaAPIVK		();
};

#endif
