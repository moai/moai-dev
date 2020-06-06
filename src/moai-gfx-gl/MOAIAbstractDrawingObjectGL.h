// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDRAWINGOBJECTGL_H
#define	MOAIABSTRACTDRAWINGOBJECTGL_H

#include <moai-gfx-gl/MOAIAbstractDrawingAPIGL.h>

//================================================================//
// MOAIAbstractDrawingObjectGL
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingObjectGL :
	public virtual MOAIAbstractDrawingAPIGL,
	public virtual MOAIAbstractDrawingObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
 	static int			_loadShaderUniformGL				( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
						MOAIAbstractDrawingObjectGL			();
	virtual				~MOAIAbstractDrawingObjectGL		();
};

#endif
