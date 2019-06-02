// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFERGL_H
#define	MOAIVERTEXBUFFERGL_H

#include <moai-gfx-gl/MOAIGfxBufferGL.h>

class MOAIVertexFormatGL;

//================================================================//
// MOAIVertexBufferGL
//================================================================//
/**	@lua	MOAIIndexBufferGL
	@text	Buffer for vertices.
*/
class MOAIVertexBufferGL :
	public virtual MOAIVertexBuffer,
	public virtual MOAIGfxBufferGL {
private:
	
	friend class MOAIGfxMgrGL_PipelineClerkGL;
	friend class MOAIVertexBufferWithFormatGL;
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBufferGL )
	
	//----------------------------------------------------------------//
					MOAIVertexBufferGL		();
					~MOAIVertexBufferGL		();
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn				( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
};

#endif
