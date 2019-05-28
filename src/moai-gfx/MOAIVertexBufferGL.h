// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFERGL_H
#define	MOAIVERTEXBUFFERGL_H

#include <moai-gfx/MOAIGfxResourceGL.h>
#include <moai-gfx/MOAIAbstractGfxBufferGL.h>

class MOAIVertexFormatGL;

//================================================================//
// MOAIVertexBufferGL
//================================================================//
/**	@lua	MOAIIndexBufferGL
	@text	Buffer for vertices.
*/
class MOAIVertexBufferGL :
	public virtual MOAIGfxResourceGL < ZLVertexBufferGL >,
	public virtual MOAIAbstractGfxBufferGL {
private:
	
	friend class ZLGfxPipelineClerkGL;
	friend class ZLVertexBufferWithFormatGL;

	//----------------------------------------------------------------//
	static int		_computeBounds			( lua_State* L );
	static int		_countElements			( lua_State* L );
	static int		_printVertices			( lua_State* L );
	
	//----------------------------------------------------------------//
	ZLGfxBufferGL&		MOAIGfxBuffer_ZLGfxBuffer		();
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBufferGL )
	
	//----------------------------------------------------------------//
					MOAIVertexBufferGL		();
					~MOAIVertexBufferGL		();
	void			PrintVertices			( MOAIVertexFormatGL& vertexFormat );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
