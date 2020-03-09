// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFER_H
#define	MOAIVERTEXBUFFER_H

#include <moai-gfx/MOAIGfxBuffer.h>

class MOAIVertexFormat;

//================================================================//
// MOAIVertexBuffer
//================================================================//
class MOAIVertexBuffer :
	public virtual MOAIGfxBuffer {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
	static int		_computeBounds			( lua_State* L );
	static int		_countElements			( lua_State* L );
	static int		_printVertices			( lua_State* L );
	
	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	
public:

	//----------------------------------------------------------------//
					MOAIVertexBuffer		();
					~MOAIVertexBuffer		();
	void			PrintVertices			( MOAIVertexFormat& vertexFormat );
};

#endif
