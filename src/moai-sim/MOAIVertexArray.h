// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXARRAY_H
#define	MOAIVERTEXARRAY_H

#include <moai-sim/ZLVertexArray.h>

class MOAIVertexArray;
class MOAIVertexBuffer;
class ZLVertexBufferWithFormat;
class MOAIVertexFormat;

//================================================================//
// MOAIVertexArray
//================================================================//
// TODO: doxygen
class MOAIVertexArray :
	public MOAIGfxResource < ZLVertexArray > {
protected:

	//----------------------------------------------------------------//
	static int			_reserveVAOs				( lua_State* L );
	static int			_reserveVertexBuffers		( lua_State* L );
	static int			_setVertexBuffer			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIVertexArray )
	
	//----------------------------------------------------------------//
						MOAIVertexArray				();
						~MOAIVertexArray			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
