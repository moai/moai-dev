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
	public virtual MOAIGfxBuffer{
protected:

	//----------------------------------------------------------------//
	static int		_computeBounds			( lua_State* L );
	static int		_countElements			( lua_State* L );
	static int		_printVertices			( lua_State* L );
	
public:

	//----------------------------------------------------------------//
					MOAIVertexBuffer		();
					~MOAIVertexBuffer		();
	void			PrintVertices			( MOAIVertexFormat& vertexFormat );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
