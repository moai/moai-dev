// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDRAWINGLUAAPIGL_H
#define	MOAIABSTRACTDRAWINGLUAAPIGL_H

#include <moai-gfx-gl/MOAIAbstractDrawingAPIGL.h>

//================================================================//
// MOAIAbstractDrawingLuaAPIGL
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingLuaAPIGL :
	public virtual MOAIAbstractDrawingAPIGL,
	public virtual MOAIAbstractDrawingLuaAPI {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
 	static int			_loadShaderUniformGL				( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
						MOAIAbstractDrawingLuaAPIGL			();
	virtual				~MOAIAbstractDrawingLuaAPIGL		();
};

#endif
