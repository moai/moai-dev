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
	
	friend class MOAIGfxMgrGL_DisplayListClerkGL;
	friend class MOAIVertexBufferWithFormatGL;
	
	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBufferGL )
	
	//----------------------------------------------------------------//
					MOAIVertexBufferGL		();
					~MOAIVertexBufferGL		();
};

#endif
